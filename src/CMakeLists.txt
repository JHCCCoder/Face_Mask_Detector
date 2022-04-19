set (LIB_SRC
        bbox_tracker.cpp
        face_mask_detector.cpp)

# opencv packages
find_package (OpenCV REQUIRED)

# build library for test
add_library (FaceMaskDetectionLib ${LIB_SRC})
target_link_libraries (FaceMaskDetectionLib ${OpenCV_LIBS} tensorflow-lite)

add_executable (FaceMaskDetector main.cpp)
target_link_libraries (FaceMaskDetector FaceMaskDetectionLib ${OpenCV_LIBS} tensorflow-lite dl)
target_include_directories(FaceMaskDetector PRIVATE "${FaceMaskDetector_SOURCE_DIR}/../flatbuffers/include")
