#define BOOST_TEST_MODULE MaskDetectorTest

#include <boost/test/unit_test.hpp>

#include "mask_detector.h"
#include "face_detector.h"
#include "data_structures.h"

using namespace paddle::lite_api;

BOOST_AUTO_TEST_CASE(DetectFromImageTest) {
    std::string model_path = "assets";
    FaceDetector detector(model_path, 320, 240);

    Image image({cv::imread("test/test_img.jpg")});
    detector.detect(image.frame, image.faceList);

    MaskDetectorSetting setting({
        128, 128, 1.f / 256,
        0.5f, "assets/model.nb"
    });
    MaskDetector maskDetector(std::move(setting));

    maskDetector.detect(image);

    int mask_cnt = 0;
    for (auto face : image.faceList) {
        BOOST_CHECK_NE(face.maskScore, 0);
        if (face.isWearingMask) mask_cnt++;
    }

    BOOST_CHECK_NE(mask_cnt, 0);
    BOOST_CHECK_EQUAL(mask_cnt, 3);
}

BOOST_AUTO_TEST_CASE(DetectFromImageFailTest) {
    std::string model_path = "assets";
    FaceDetector detector(model_path, 320, 240);

    Image image({cv::imread("test/test_img.jpg"),
                 { FaceInfo({0, 0, 5, 5}) }
    });
    MaskDetectorSetting setting({
            128, 128, 1.f / 256,
            0.5f, "assets/model.nb"
    });
    MaskDetector maskDetector(std::move(setting));
    maskDetector.detect(image);

    BOOST_CHECK_NE(image.faceList[0].isWearingMask, false);
}