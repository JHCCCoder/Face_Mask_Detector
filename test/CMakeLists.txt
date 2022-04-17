find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
find_package (OpenCV REQUIRED)
add_compile_definitions (BOOST_TEST_DYN_LINK)
include_directories (${FaceMaskDetector_SOURCE_DIR}/src
        ${PADDLE_LITE_DIR}/cxx/include
)

add_executable (FaceDetectorTest face_detector_tests.cpp)
target_link_libraries (FaceDetectorTest
        FaceMaskDetectionLib
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${OpenCV_LIBS}
        )

add_executable (MaskDetectorTest mask_detector_tests.cpp)
target_link_libraries(MaskDetectorTest
        FaceMaskDetectionLib
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${OpenCV_LIBS}
        )