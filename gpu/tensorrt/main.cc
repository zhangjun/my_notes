
#include <iostream>
#include <cuda_runtime_api.h>
#include <vector>
#include <assert.h>

#include "NvInfer.h"
#include "NvInferPlugin.h"
#include "util.h"

using namespace nvinfer1;

class Logger : public ILogger
{
public:
    void log(Severity severity, const char* msg) noexcept override
    {
        // suppress info-level messages
        if (severity <= Severity::kERROR)
            std::cout << msg << std::endl;
    }
};

int main() {
  Logger logger;

  // cuda stream
  cudaStream_t stream;
  cudaStreamCreate(&stream);

  // Create a Network Definition
  IBuilder* builder = createInferBuilder(logger);
  nvinfer1::NetworkDefinitionCreationFlags flags =
    (1U << static_cast<int>(nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH));
  INetworkDefinition* network = builder->createNetworkV2(flags);
  

  // Add the Input layer to the network
  int batch_size = 3;
  Dims3 input_shape{4, 2, 2};
  auto input_data = network->addInput("input", DataType::kFLOAT, input_shape);
  
  // Add the plugin layer with hidden layer
  nvinfer1::IActivationLayer* act_layer = nullptr;
  act_layer = network->addActivation(*input_data, nvinfer1::ActivationType::kRELU);

  // Add a name for the output of the layer so that the tensor can be bound to a memory buffer at inference time:
  act_layer -> getOutput(0)->setName("relu:output");

  // Mark it as the output of the entire network:
  network->markOutput(*act_layer->getOutput(0));

  
  // Building an Engine(optimize the network)
  nvinfer1::IBuilderConfig* builder_config = builder->createBuilderConfig();
  
  size_t max_workspace_size = 1 << 32, max_batch_size = 1;
  builder_config->setMaxWorkspaceSize(max_workspace_size);

  bool fp16 = builder->platformHasFastFp16();
  bool int8 = builder->platformHasFastInt8();

  if (fp16) {
    // builder_config->setFlag(nvinfer1::BuilderFlag::kFP16);
  }
  if (int8) {
    // builder_config->setFlag(nvinfer1::BuilderFlag::kFP16);
    // builder_config->setFlag(nvinfer1::BuilderFlag::kINT8);
  }

  // serialization and deserialization
  std::cout << "BuildSerializedNetwork" << std::endl;
  IHostMemory*  serializedModel = builder->buildSerializedNetwork(*network, *builder_config);
  IRuntime* runtime = createInferRuntime(logger);

  std::cout << "DeserializeCudaEngine" << std::endl;
  ICudaEngine* engine = runtime->deserializeCudaEngine(serializedModel->data(), serializedModel->size());

  // Prepare input_data
  int32_t inputIndex = engine->getBindingIndex("input");
  int32_t outputIndex = engine->getBindingIndex("relu:output");
  
  std::vector<float> input(batch_size * ProductOfDims(input_shape), -1.0);
  std::vector<float> output(batch_size * ProductOfDims(engine->getBindingDimensions(outputIndex)));

  void *GPU_input_Buffer_ptr;  // a host ptr point to a GPU buffer
  void *GPU_output_Buffer_ptr;  // a host ptr point to a GPU buffer

  std::vector<void *> buffers(engine->getNbBindings());
  cudaMalloc(&GPU_input_Buffer_ptr, sizeof(float)*input.size()); //malloc gpu buffer for input
  cudaMalloc(&GPU_output_Buffer_ptr, sizeof(float)*output.size()); //malloc gpu buffer for output
  cudaMemcpy(GPU_input_Buffer_ptr, input.data(), input.size()*sizeof(float), cudaMemcpyHostToDevice); // copy input data from cpu to gpu
  
  buffers[inputIndex] = static_cast<void*>(GPU_input_Buffer_ptr);
  buffers[outputIndex] = static_cast<void*>(GPU_output_Buffer_ptr);

  // Performing Inference
  IExecutionContext *context = engine->createExecutionContext();
  std::cout << "Execute" << std::endl;
  context->enqueueV2(buffers.data(), stream, nullptr);

  // copy result data from gpu to cpu
  cudaMemcpy(output.data(), GPU_output_Buffer_ptr, output.size()*sizeof(float), cudaMemcpyDeviceToHost); 
  cudaStreamSynchronize(stream);

  // display output
  std::cout << "output shape : " << DimsToStr(engine->getBindingDimensions(outputIndex)) << "\n";
  std::cout << "output data : \n";
  for(auto i : output)
    std::cout << i << " ";
  std::cout << std::endl;
}
