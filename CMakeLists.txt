set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8)

project(FaceMaskDetector)
add_subdirectory(src)
add_subdirectory(test)
enable_testing()

add_test (NAME MyTest COMMAND Test)
add_test (NAME FaceDetectorTest COMMAND FaceDetectorTest)
add_test (NAME BboxTrackerTest COMMAND BboxTrackerTest)