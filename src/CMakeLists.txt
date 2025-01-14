set (LIB_SRC
        bbox_tracker.cpp
        face_mask_detector.cpp
        pipeline.cpp
        cgi_callback.cpp
        default_settings.cpp
        mysql_connection.cpp)

# opencv packages
find_package (OpenCV REQUIRED)
include_directories("${OpenCV_INCLUDE_DIRS}")

# include Tensorflow lite
if(NOT TENSORFLOW_SOURCE_DIR)
    get_filename_component(TENSORFLOW_SOURCE_DIR
            "${FaceMaskDetector_SOURCE_DIR}/../tensorflow-2.6.0" ABSOLUTE)
endif()
include_directories("${TENSORFLOW_SOURCE_DIR}")
include_directories("${TENSORFLOW_SOURCE_DIR}/tensorflow/lite")
link_directories(${FaceMaskDetector_SOURCE_DIR}/../tfl_build/rpi_armv7l/lib)

# include flatbuffers
SET(FLAT_BUFFERS_DIR
        ${FaceMaskDetector_SOURCE_DIR}/../flatbuffers)

set(CMAKE_MODULE_PATH ${FaceMaskDetector_SOURCE_DIR})
find_package(MySQL REQUIRED)
include_directories(${MYSQL_INCLUDE_DIRS})
message(STATUS "${MYSQL_INCLUDE_DIRS}")
message(STATUS "${MYSQL_LIBRARIES}")

# build library for test
add_library (FaceMaskDetectionLib STATIC ${LIB_SRC})
target_link_libraries (FaceMaskDetectionLib ${MYSQL_LIBRARIES})
target_include_directories(FaceMaskDetectionLib PUBLIC "${FLAT_BUFFERS_DIR}/include")

add_executable (FaceMaskDetector main.cpp)
target_link_libraries (FaceMaskDetector FaceMaskDetectionLib ${OpenCV_LIBS} tensorflow-lite dl)
target_link_libraries (FaceMaskDetector cpptimer rt fcgi curl ${MYSQL_LIBRARIES})
target_include_directories(FaceMaskDetector PRIVATE "${FLAT_BUFFERS_DIR}/include")
