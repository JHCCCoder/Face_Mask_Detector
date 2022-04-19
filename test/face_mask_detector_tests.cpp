#define BOOST_TEST_MODULE FaceMaskDetectorTest

#include <boost/test/unit_test.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

#include "face_mask_detector.h"

BOOST_AUTO_TEST_CASE(DetectImageTest) {
    std::string model_path = "assets";
    FaceMaskDetector detector(model_path, 0.5f);

    auto image = cv::imread("test/test_img.jpg");
    auto result = detector.process(image);

    BOOST_CHECK_EQUAL(result.faceList.size(), 5);
}
