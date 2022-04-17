set (CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 2.8)

project(FaceMaskDetector)

# include Paddle Lite
set (PADDLE_LITE_DIR
#        ${FaceMaskDetector_SOURCE_DIR}/lib/paddle-lite)
        /Users/minhao/Downloads/inference_lite_lib_2.6.3/cxx)
include_directories (${PADDLE_LITE_DIR}/include)
link_directories (${PADDLE_LITE_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test)
enable_testing()

add_test (NAME FaceDetectorTest COMMAND FaceDetectorTest)
add_test (NAME MaskDetectorTest COMMAND MaskDetectorTest)
