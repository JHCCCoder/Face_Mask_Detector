find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
find_package (OpenCV REQUIRED)
add_definitions (-DBOOST_TEST_DYN_LINK)
include_directories (${FaceMaskDetector_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)

add_executable (Test test.cpp)
target_link_libraries (Test
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

add_executable (FaceDetectorTest face_detector_tests.cpp)
target_link_libraries (FaceDetectorTest
        FaceMaskDetectionLib
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${OpenCV_LIBS}
        )

add_executable (BboxTrackerTest bbox_tracker_tests.cpp)
target_link_libraries (BboxTrackerTest
        FaceMaskDetectionLib
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${OpenCV_LIBS}
        )