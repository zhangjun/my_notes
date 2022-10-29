import pycuda.driver as cuda
import pycuda.autoinit
import numpy as np
import tensorrt as trt

input_shape = (1,4)

runtime = trt.Runtime(TRT_LOGGER)

# restore engine
with open('engine', 'rb') as f:
    engine = runtime.deserialize_cuda_engine(f.read())

# make context
context = engine.create_execution_context()

# allocate input and output memory
input_nbytes = trt.volume(input_shape)* trt.int32.itemsize
d_input = cuda.mem_alloc(input_nbytes)
context.set_binding_shape(0, input_shape)
h_output = cuda.pagelocked_empty(tuple(context.get_binding_shape(0)), dtype=np.int32)
d_output = cuda.mem_alloc(h_output.nbytes)
stream = cuda.Stream()  # Create a stream in which to copy inputs/outputs and run inference.

# setup input
data = np.array([[1,2,3,4]], dtype='int32')
cuda.memcpy_htod_async(d_input, data, stream)

# do inference
context.execute_async_v2(bindings=[int(d_input)] + [int(d_output)], stream_handle=stream.handle)

# copy results to host
cuda.memcpy_dtoh_async(h_output, d_output, stream)
stream.synchronize()

# show result
print(h_output)