git submodule update --init --recursive

CMAKE_CUDA_ARCHITECTURES="75;80"

./build.sh --parallel --skip_submodule_sync --use_dnnl --cuda_home /usr/local/cuda --cudnn_home /usr --use_tensorrt --tensorrt_home /usr/local/TensorRT-8.4.0.6 --config Release --build_wheel --skip_tests --update --build --cmake_extra_defines ONNXRUNTIME_VERSION=$(cat ./VERSION_NUMBER) '"CMAKE_CUDA_ARCHITECTURES='${CMAKE_CUDA_ARCHITECTURES}'"' CMAKE_CXX_COMPILER=/usr/local/gcc-8.2/bin/g++ CMAKE_C_COMPILER=/usr/local/gcc-8.2/bin/gcc

