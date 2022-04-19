set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
cmake_minimum_required(VERSION 3.16)

project(FaceMaskDetector)

# include Tensorflow lite
if(NOT TENSORFLOW_SOURCE_DIR)
    get_filename_component(TENSORFLOW_SOURCE_DIR
            "${FaceMaskDetector_SOURCE_DIR}/../tensorflow-2.6.0" ABSOLUTE)
    message(STATUS "${TENSORFLOW_SOURCE_DIR} test")
endif()
include_directories("${TENSORFLOW_SOURCE_DIR}")
include_directories("${TENSORFLOW_SOURCE_DIR}/tensorflow/lite")
SET (TFL_BUILD_DIR
        ${FaceMaskDetector_SOURCE_DIR}/../tfl_build)
link_directories (${TFL_BUILD_DIR}/rpi_armv7l/lib)

# include flatbuffers
SET(FLAT_BUFFERS_DIR
        ${FaceMaskDetector_SOURCE_DIR}/../flatbuffers)

add_subdirectory(src)
add_subdirectory(test)
enable_testing()


add_test (NAME FaceDetectorTest COMMAND FaceDetectorTest)
add_test (NAME MaskDetectorTest COMMAND MaskDetectorTest)
