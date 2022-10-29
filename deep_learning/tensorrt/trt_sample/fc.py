# build the engine
import tensorrt as trt
import numpy as np

## Setup Builder
TRT_LOGGER = trt.Logger(trt.Logger.INFO)
builder = trt.Builder(TRT_LOGGER)

## Build Network
batch_size = 2
input_dim = 4
output_dim = 2
network = builder.create_network(1 << int(trt.NetworkDefinitionCreationFlag.EXPLICIT_BATCH))
input_shape = (batch_size, input_dim, 1, 1)
x = network.add_input(name="X", dtype=trt.float32, shape=input_shape)

W = np.ones((input_dim, output_dim), dtype='float32')
B = np.zeros(output_dim, dtype='float32')
fc_Layer = network.add_fully_connected(x, output_dim, W, B)
y = fc_Layer.get_output(0)

network.mark_output(y)
engine = builder.build_engine(network, builder.create_builder_config())

# do inference

def nvinfer(engine, X_shape,  Y_shape, X_dtype='float32', output_dtype='float32'):
    import pycuda.driver as cuda
    import pycuda.autoinit
    import numpy as np
    import tensorrt as trt

    runtime = trt.Runtime(TRT_LOGGER)

    # make context
    context = engine.create_execution_context()

    # allocate input and output memory
    input_nbytes = trt.volume(X_shape) * getattr(trt, X_dtype).itemsize
    d_input = cuda.mem_alloc(input_nbytes)
    context.set_binding_shape(0, X.shape)
    h_output = cuda.pagelocked_empty(Y_shape, dtype=output_dtype)
    d_output = cuda.mem_alloc(h_output.nbytes)
    stream = cuda.Stream()  # Create a stream in which to copy inputs/outputs and run inference.

    def run(X):
        cuda.memcpy_htod_async(d_input, X, stream)
        # do inference
        context.execute_async_v2(bindings=[int(d_input)] + [int(d_output)], stream_handle=stream.handle)
        # copy results to host
        cuda.memcpy_dtoh_async(h_output, d_output, stream)
        stream.synchronize()
        return h_output
    return run

run = nvinfer(engine, (2,4,), [2,2])
X = np.ones([2,4], dtype='float32')
print(run(X) - np.matmul(X, W))