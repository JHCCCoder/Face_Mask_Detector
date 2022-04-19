find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
find_package (OpenCV REQUIRED)
add_compile_definitions (BOOST_TEST_DYN_LINK)

include_directories("${FaceMaskDetector_SOURCE_DIR}/src"
        ${Boost_INCLUDE_DIRS})

# include Tensorflow lite
if(NOT TENSORFLOW_SOURCE_DIR)
    get_filename_component(TENSORFLOW_SOURCE_DIR
            "${FaceMaskDetector_SOURCE_DIR}/../tensorflow-2.6.0" ABSOLUTE)
endif()
include_directories("${TENSORFLOW_SOURCE_DIR}")
include_directories("${TENSORFLOW_SOURCE_DIR}/tensorflow/lite")
SET (TFL_BUILD_DIR
        ${FaceMaskDetector_SOURCE_DIR}/../tfl_build)
link_directories (${TFL_BUILD_DIR}/rpi_armv7l/lib)

add_executable (BboxTrackerTest bbox_tracker_tests.cpp)
target_link_libraries (BboxTrackerTest
        FaceMaskDetectionLib
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${OpenCV_LIBS}
        )

add_executable (FaceMaskDetectorTest face_mask_detector_tests.cpp)
target_link_libraries (FaceMaskDetectorTest
        FaceMaskDetectionLib
        tensorflow-lite
        dl
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${OpenCV_LIBS}
        )
