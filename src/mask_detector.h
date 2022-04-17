#ifndef MASK_DETECTOR_H_
#define MASK_DETECTOR_H_

#include <opencv2/opencv.hpp>
#include <paddle_api.h>
#include <paddle_use_kernels.h>
#include <paddle_use_ops.h>

#include "data_structures.h"

#define COLOR_MASK cv::Scalar(0, 255, 0)
#define COLOR_NO_MASK cv::Scalar(0, 0, 255)

using namespace paddle::lite_api;

class MaskDetector {
public:
    MaskDetector(MaskDetectorSetting&& mdSetting);

    void detect(Image& image);
    cv::Mat drawFaceMaskRects(Image& image);

private:
    MobileConfig mobileConfig;
    MaskDetectorSetting maskDetectionSetting;
    std::shared_ptr<PaddlePredictor> predictor;
    std::unique_ptr<Tensor> inputTensor;
    std::unique_ptr<const Tensor> outputTensor;

    // region of interest
    cv::Mat normalise_roi(const cv::Mat& frame, FaceInfo face);
};

#endif // MASK_DETECTOR_H_