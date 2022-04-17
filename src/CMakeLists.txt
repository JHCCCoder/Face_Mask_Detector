set (LIB_SRC
        bbox_tracker.cpp
        face_detector.cpp
        mask_detector.cpp)

# opencv packages
find_package (OpenCV REQUIRED)

# build library for test
add_library (FaceMaskDetectionLib ${LIB_SRC})
target_link_libraries (FaceMaskDetectionLib ${OpenCV_LIBS} paddle_light_api_shared)

add_executable (FaceMaskDetector main.cpp)
target_link_libraries (FaceMaskDetector FaceMaskDetectionLib ${OpenCV_LIBS} paddle_light_api_shared)
