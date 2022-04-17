#define BOOST_TEST_MODULE FaceDetectorTest

#include <boost/test/unit_test.hpp>
#include <opencv2/opencv.hpp>

#include <vector>

#include "face_detector.h"
#include "data_structures.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(DetectFromImageTest) {
    std::string model_path = "assets";
    FaceDetector detector(model_path, 320, 240);

    auto image = cv::imread("test/test_img.jpg");
    std::vector<FaceInfo> face_list;
    detector.detect(image, face_list);

    BOOST_CHECK_GT(face_list.size(), 0);
    BOOST_CHECK_EQUAL(face_list.size(), 5);
}
