#include "face_mask_detector.h"

int main() {
    std::string model_path = "assets";
    FaceMaskDetector detector(model_path, 0.5f);

    auto image = cv::imread("test/test_img.jpg");
    auto result = detector.process(image);

    cv::imwrite("assets/out.jpg", result.frame);

    return 0;
}