set (LIB_SRC
        bbox_tracker.cpp
        face_mask_detector.cpp)

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

# build library for test
add_library (FaceMaskDetectionLib STATIC ${LIB_SRC})
target_include_directories(FaceMaskDetectionLib PUBLIC "${FLAT_BUFFERS_DIR}/include")

add_executable (FaceMaskDetector main.cpp)
target_link_libraries (FaceMaskDetector FaceMaskDetectionLib ${OpenCV_LIBS} tensorflow-lite dl)
target_include_directories(FaceMaskDetector PRIVATE "${FLAT_BUFFERS_DIR}/include")
