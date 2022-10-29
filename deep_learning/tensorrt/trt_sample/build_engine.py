import tensorrt as trt

## Setup Builder
TRT_LOGGER = trt.Logger(trt.Logger.INFO)
builder = trt.Builder(TRT_LOGGER)
explicit_batch_flag = 1 << int(trt.NetworkDefinitionCreationFlag.EXPLICIT_BATCH)
network = builder.create_network(explicit_batch_flag)
builder_config = builder.create_builder_config()
builder_config.max_workspace_size = 5000 * (1024 * 1024) # 5000 MiB

## Build Network
input_shape = (1,4)
x = network.add_input(name="X", dtype=trt.int32, shape=input_shape)
id_layer = network.add_identity(x)
x = id_layer.get_output(0)
network.mark_output(x)

## Get Engine
engine = builder.build_engine(network, builder_config)

## Write engine
with open('engine', 'wb') as f:
    f.write(engine.serialize())