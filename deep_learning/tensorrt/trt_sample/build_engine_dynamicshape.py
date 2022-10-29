import tensorrt as trt
import numpy as np

GGER = trt.Logger(trt.Logger.INFO)
builder = trt.Builder(TRT_LOGGER)
explicit_batch_flag = 1 << int(trt.NetworkDefinitionCreationFlag.EXPLICIT_BATCH)
network = builder.create_network(explicit_batch_flag)
builder_config = builder.create_builder_config()
builder_config.max_workspace_size = 5000 * (1024 * 1024) # 5000 MiB

profile = builder.create_optimization_profile()
profile.set_shape("X", min=[1,4], opt=[1,4], max=[1,4])
builder_config.add_optimization_profile(profile)

# Input Layer
input_shape = (-1,4)
x = network.add_input(name="X", dtype=trt.int32, shape=input_shape)

# Identity Layer
id_layer = network.add_identity(x)
x = id_layer.get_output(0)

indices = network.add_constant([1], np.array([0], dtype='int32')).get_output(0)
axis = 1
gather_layer = network.add_gather(x, indices, axis)
x = gather_layer.get_output(0)
print(f"shape: {x.shape}")

network.mark_output(x)
engine = builder.build_engine(network, builder_config)

## Write engine
with open('dynamic.engine', 'wb') as f:
    f.write(engine.serialize())