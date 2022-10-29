import tensorrt as trt

engine_file = 'fp32.engine'
trt_logger = trt.Logger()
runtime = trt.Runtime(trt_logger)
f = open(engine_file, 'rb')
engine = runtime.deserialize_cuda_engine(f.read())