set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
cmake_minimum_required(VERSION 3.16)

project(FaceMaskDetector)

add_subdirectory(src)
add_subdirectory(test)
enable_testing()

add_test (NAME BboxTrackerTest COMMAND BboxTrackerTest)
add_test (NAME FaceMaskDetectorTest COMMAND FaceMaskDetectorTest)
