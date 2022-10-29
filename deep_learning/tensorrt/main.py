from itertools import chain
import argparse
import os
import pycuda.driver as cuda
import pycuda.autoinit
import numpy as np
import tensorrt as trt

import threading

# Simple helper data class that's a little nicer to use than a 2-tuple.
class HostDeviceMem(object):
    def __init__(self, host_mem, device_mem):
        self.host = host_mem
        self.device = device_mem


    def __str__(self):
        return "Host:\n" + str(self.host) + "\nDevice:\n" + str(self.device)

    def __repr__(self):
        return self.__str__()

# Allocates all buffers required for an engine, i.e. host/device inputs/outputs.
def allocate_buffers(engine):
    inputs = []
    outputs = []
    bindings = []
    stream = cuda.Stream()
    for binding in engine:
        size = trt.volume(engine.get_binding_shape(binding)) * engine.max_batch_size
        dtype = trt.nptype(engine.get_binding_dtype(binding))
        # Allocate host and device buffers
        host_mem = cuda.pagelocked_empty(size, dtype)
        device_mem = cuda.mem_alloc(host_mem.nbytes)
        # Append the device buffer to device bindings.
        bindings.append(int(device_mem))
        # Append to the appropriate list.
        if engine.binding_is_input(binding):
            inputs.append(HostDeviceMem(host_mem, device_mem))
        else:
            outputs.append(HostDeviceMem(host_mem, device_mem))
    # ctx.pop()
    # del ctx
    return inputs, outputs, bindings, stream

# This function is generalized for multiple inputs/outputs.
# inputs and outputs are expected to be lists of HostDeviceMem objects.
def do_inference(context, bindings, inputs, outputs, batch_size=1):
    # Transfer input data to the GPU.
    [cuda.memcpy_htod_async(inp.device, inp.host, stream) for inp in inputs]
    # [cuda.memcpy_htod(inp.device, inp.host) for inp in inputs]
    # Run inference.
    context.execute_async(batch_size=batch_size, bindings=bindings, stream_handle=stream.handle)
    # context.execute(batch_size=batch_size, bindings=bindings)
    # Transfer predictions back from the GPU.
    [cuda.memcpy_dtoh_async(out.host, out.device, stream) for out in outputs]
    # [cuda.memcpy_dtoh(out.host, out.device) for out in outputs]
    # Synchronize the stream
    stream.synchronize()
    # Return only the host outputs.

    return [out.host for out in outputs]

# This function is generalized for multiple inputs/outputs for full dimension networks.
# inputs and outputs are expected to be lists of HostDeviceMem objects.
def do_inference_v2(context, bindings, inputs, outputs, stream):
    # Transfer input data to the GPU.
    [cuda.memcpy_htod_async(inp.device, inp.host, stream) for inp in inputs]
    # Run inference.
    context.execute_async_v2(bindings=bindings, stream_handle=stream.handle)
    # Transfer predictions back from the GPU.
    [cuda.memcpy_dtoh_async(out.host, out.device, stream) for out in outputs]
    # Synchronize the stream
    stream.synchronize()
    # Return only the host outputs.
    return [out.host for out in outputs]

def callback(device_id):
    # cuda.init()
    device = cuda.Device(device_id)  # enter your Gpu id here
    ctx = device.make_context()

    (free,total) = cuda.mem_get_info()
    print(free, total)

    # allocate_buffers()  # load Cuda buffers or any other Cuda or TenosrRT operations
    engine_file = 'fp32.engine'
    trt_logger = trt.Logger()
    runtime = trt.Runtime(trt_logger)
    f = open(engine_file, 'rb')
    engine = runtime.deserialize_cuda_engine(f.read())
    context = engine.create_execution_context()
    inputs, outputs, bindings, stream = allocate_buffers(engine)

    fake_input = np.ones([1,3,224,224], dtype=np.float32)
    inputs[0].host = fake_input
    output = do_inference_v2(context, bindings, inputs, outputs, stream)

    ctx.pop()  # very important


if __name__ == "__main__":
    cuda.init()
    device_num = cuda.Device.count()

    thread_list = []
    for i in range(device_num):
        worker_thread = threading.Thread(target=callback(i))
        # worker_thread = threading.Thread(target=callback, args=(i,))
        worker_thread.start()
        thread_list.append(worker_thread)
    
    for i in range(device_num):
        thread_list[i].join()
