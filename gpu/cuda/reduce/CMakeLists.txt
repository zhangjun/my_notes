cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# https://developer.nvidia.com/blog/building-cuda-applications-cmake/

# set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc")
# find_package(CUDA QUIET REQUIRED)
# enable_language(CUDA)
project(main
  VERSION 0.1
  DESCRIPTION "gpu dev"
  LANGUAGES CXX CUDA)

set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

find_package(CUDA QUIET)
if(${CUDA_FOUND})
  message(STATUS "Found CUDA ${CUDA_VERSION_STRING} at ${CUDA_TOOLKIT_ROOT_DIR}")
  enable_language(CUDA)
  include(cuda)
endif()

set(CUDA_LINK_LIBRARIES_KEYWORD PUBLIC)
set_source_files_properties(test.cpp PROPERTIES LANGUAGE CUDA)

include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

# message(STATUS "CUDA_LIBRARIES: ${CUDA_LIBRARIES}")
# message(STATUS "CUDA_INCLUDE_DIR: ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")
# add_compile_options(-std=c++11)
# add_definitions(-std=c++11)

list(APPEND CUDA_NVCC_FLAGS "-std=c++11")
set(CUDA_NVCC_PLAGS ${CUDA_NVCC_PLAGS};-std=c++11;-gencode;arch=compute_80;code=sm_80)



add_executable(${PROJECT_NAME} main.cu)
target_link_libraries(${PROJECT_NAME} ${CUDA_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)



# set_target_properties(particles PROPERTIES POSITION_INDEPENDENT_CODE ON)
# set(CUDA_NVCC_PLAGS ${CUDA_NVCC_PLAGS};-std=c++11;-g;-G;-gencode;arch=compute_75;code=sm_75)
option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CUDA_SEPARABLE_COMPILATION ON)
# set(CUDA_PROPAGATE_HOST_FLAGS OFF)
