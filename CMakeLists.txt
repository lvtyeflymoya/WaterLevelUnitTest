cmake_minimum_required(VERSION 3.25.0)
project(PanoramicTracking
        VERSION 0.0.1
        DESCRIPTION "Real-time panoramic video stitching and object tracking"
        HOMEPAGE_URL "https://github.com/Li-Ruiqi777/PanoramicTracking"
        LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(Function)

# 三方库
## OpenCV
list(APPEND CMAKE_PREFIX_PATH "D:/apps/Opencv/opencv/build")
find_package(OpenCV REQUIRED)

# PLOG
list(APPEND CMAKE_PREFIX_PATH "D:/ThirdPart/plog")
find_package(plog REQUIRED)

## CUDA
find_package(CUDA REQUIRED)
message("CUDA_LIBRARY_DIRS: ${CUDA_LIBRARY_DIRS}")

# TensorRT
set(TRT_DIR "D:/ThirdPart/TensorRT-10.8.0.43")

link_directories(${TRT_DIR}/lib)

# 设置可执行文件和动态库文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

list(APPEND ALL_INCLUDE_DIRS
        ${OpenCV_INCLUDE_DIRS}
        ${CUDA_INCLUDE_DIRS}
        D:/ThirdPart/plog/include
        ${TRT_DIR}/include
)


list(APPEND ALL_LIBS
        ${OpenCV_LIBRARIES}
        ${CUDA_LIBRARIES}
        nvinfer_10
        nvinfer_plugin_10
        nvonnxparser_10
)

# 添加子目录
add_subdirectory(src)
add_subdirectory(test)

# 链接库
target_include_directories(${PROJECT_NAME} PRIVATE ${ALL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ALL_LIBS})

# 设置安装规则
set(CMAKE_INSTALL_PREFIX "D:/Cpp_Project/install/")
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION bin
        RUNTIME DESTINATION bin
        PRIVATE_HEADER DESTINATION include
        PUBLIC_HEADER DESTINATION include)
