cmake_minimum_required(VERSION 3.17)
project(GVirtuS)


# 设置构建类型为 Debug，启用调试信息
set(CMAKE_BUILD_TYPE Debug)

# 为 C++ 和 CUDA 添加调试选项
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")  # C++ 标志，-g 为调试信息，-O0 禁用优化
set(CMAKE_CUDA_FLAGS_DEBUG "-G -g -O0")  # CUDA 标志，-G 用于调试，-g 为调试信息，-O0 禁用优化

# 如果需要强制将调试选项添加到所有目标，可以使用如下方式：
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} ${CMAKE_CUDA_FLAGS_DEBUG}")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)

if("$ENV{GVIRTUS_HOME}" STREQUAL "")
    message(STATUS "Setting GVIRTUS_HOME=$ENV{HOME}/GVirtuS")
    set(GVIRTUS_HOME "$ENV{HOME}/GVirtuS")
    set($ENV{GVIRTUS_HOME} "${GVIRTUS_HOME}")
else()
    message(STATUS "Using GVIRTUS_HOME=$ENV{GVIRTUS_HOME}")
    set(GVIRTUS_HOME "$ENV{GVIRTUS_HOME}")
endif()

include(cmake/GVirtuS.cmake)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/nlohmann/json.hpp
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/include/nlohmann
        COMMAND curl -L https://github.com/nlohmann/json/releases/download/v3.7.3/json.hpp -o ${CMAKE_CURRENT_BINARY_DIR}/include/nlohmann/json.hpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_library(gvirtus-common SHARED
        ${CMAKE_CURRENT_BINARY_DIR}/include/nlohmann/json.hpp
        src/common/Decoder.cpp
        src/common/Encoder.cpp
        src/common/JSON.cpp
        src/common/LD_Lib.cpp
        src/common/MessageDispatcher.cpp
        src/common/Mutex.cpp
        src/common/Observable.cpp
        src/common/Observer.cpp
        src/common/SignalException.cpp
        src/common/SignalState.cpp
        ##src/communicators/rdma/ktmrdma.cpp
        src/common/Util.cpp)

add_dependencies(gvirtus-common log4cplus)

target_link_libraries(gvirtus-common stdc++fs ${CMAKE_DL_LIBS} ${LIBLOG4CPLUS} rdmacm ibverbs)

##target_include_directories(gvirtus-common PRIVATE /usr/include/infiniband)
##target_include_directories(gvirtus-common PRIVATE /usr/include/rdma)

gvirtus_install_target(gvirtus-common)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/nlohmann DESTINATION ${GVIRTUS_HOME}/include)
install(DIRECTORY ${EXTERNAL_INSTALL_LOCATION}/include/log4cplus DESTINATION ${GVIRTUS_HOME}/include)
install(DIRECTORY ${EXTERNAL_INSTALL_LOCATION}/lib DESTINATION ${GVIRTUS_HOME})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/etc DESTINATION ${GVIRTUS_HOME})

add_library(gvirtus-communicators SHARED
        src/communicators/Buffer.cpp
        src/communicators/CommunicatorFactory.cpp
        src/communicators/Endpoint_Tcp.cpp
        src/communicators/Endpoint_Rdma.cpp
        src/communicators/EndpointFactory.cpp
        src/communicators/rdma/ktmrdma.cpp
        src/communicators/Result.cpp
        )
target_link_libraries(gvirtus-communicators gvirtus-common rdmacm ibverbs)
gvirtus_install_target(gvirtus-communicators)

## TCP COMMUNICATOR
add_library(gvirtus-communicators-tcp SHARED
        src/communicators/tcp/TcpCommunicator.cpp)
target_link_libraries(gvirtus-communicators-tcp gvirtus-communicators)
gvirtus_install_target(gvirtus-communicators-tcp)

## IB COMMUNICATOR
## TODO: PROPERLY SETUP CMAKE AND PROJECT TO USE RDMA
add_library(gvirtus-communicators-ib SHARED
        src/communicators/rdma/ktmrdma.cpp
        src/communicators/rdma/RdmaCommunicator.cpp
        )
##target_include_directories(gvirtus-communicators-ib PUBLIC /usr/include/infiniband /usr/include/rdma /src/communicators/rdma)
target_link_libraries(gvirtus-communicators-ib gvirtus-communicators rdmacm ibverbs)
gvirtus_install_target(gvirtus-communicators-ib)

add_library(gvirtus-frontend SHARED
        src/frontend/Frontend.cpp)
target_link_libraries(gvirtus-frontend gvirtus-communicators Threads::Threads)
gvirtus_install_target(gvirtus-frontend)

add_executable(gvirtus-backend
        src/backend/Backend.cpp
        src/backend/main.cpp
        src/backend/Process.cpp
        src/backend/Property.cpp)
target_link_libraries(gvirtus-backend gvirtus-communicators Threads::Threads rdmacm ibverbs)
gvirtus_install_target(gvirtus-backend)

# add_subdirectory(plugins/cublas)
add_subdirectory(plugins/cudart)
# add_subdirectory(plugins/cufft)
# add_subdirectory(plugins/curand)
# add_subdirectory(plugins/cudnn)
#add_subdirectory(plugins/cusolver)
#add_subdirectory(plugins/cusparse)

#add_subdirectory(tools/protocol-generator)

