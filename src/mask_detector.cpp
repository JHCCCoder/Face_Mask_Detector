#include "mask_detector.h"

MaskDetector::MaskDetector(MaskDetectorSetting&& mdSetting) : maskDetectionSetting(mdSetting){
    mobileConfig.set_model_from_file(mdSetting.modelPath);
    predictor = CreatePaddlePredictor<MobileConfig>(mobileConfig);
    inputTensor = std::move(predictor->GetInput(0));
    outputTensor = std::move(predictor->GetOutput(0));

    inputTensor->Resize({1, 3, mdSetting.normalisedHeight, mdSetting.normalisedWidth});
}

void MaskDetector::detect(Image& image) {
    auto* input_data = inputTensor->mutable_data<float>();
    auto* output_data = outputTensor->data<float>();

    for (auto face : image.faceList) {
        auto normalised_roi = normalise_roi(image.frame, face);
        if (!normalised_roi.empty()) {
            auto* img = reinterpret_cast<const float*>(normalised_roi.data);

            float* out_c0 = input_data;
            float* out_c1 = input_data + maskDetectionSetting.imageSize();
            float* out_c2 = input_data + maskDetectionSetting.imageSize() * 2;

            for (int i = 0; i < maskDetectionSetting.imageSize(); i++) {
                *(out_c0++) = (*(img++) - 0.5f);
                *(out_c1++) = (*(img++) - 0.5f);
                *(out_c2++) = (*(img++) - 0.5f);
            }

            predictor->Run();

            float score = output_data[1];
            face.maskScore = score;
            face.isWearingMask = score > maskDetectionSetting.scoreThreshold;
        }
    }
}

cv::Mat MaskDetector::drawFaceMaskRects(Image& image) {
    cv::Mat processed_frame(image.frame);
    for (const auto& face : image.faceList) {
        if (face.maskScore == 0) continue;

        cv::Rect2f faceRect(face.topLeft, face.bottomRight);
        cv::rectangle(processed_frame,
                      faceRect,
                      face.isWearingMask ? COLOR_MASK : COLOR_NO_MASK,
                      2);
    }
    return processed_frame;
}

cv::Mat MaskDetector::normalise_roi(const cv::Mat &frame, FaceInfo face) {
    // todo: remove the enlarge piece to face detector
    // enlarge face rect
    float width_offset = (face.x2 - face.x1) / 20.f;
    float height_offset = (face.y2 - face.y1) / 20.f;
    cv::Point2f pt1(std::max(face.x1 - width_offset, 0.0f), std::max(face.y1 - height_offset, 0.0f));
    cv::Point2f pt2(std::min(face.x2 + width_offset, float(frame.cols)), std::min(face.y2 + height_offset, float(frame.rows)));

    cv::Rect2f rectClip(pt1, pt2);

    cv::Mat resized_img, normalised_roi;
    if (rectClip.width > 0 && rectClip.height > 0) {
        cv::Mat roi = frame(rectClip);

        cv::resize(
                roi,
                resized_img,
                cv::Size(maskDetectionSetting.normalisedWidth, maskDetectionSetting.normalisedHeight),
                0.f,
                0.f,
                cv::INTER_CUBIC);

        resized_img.convertTo(normalised_roi, CV_32FC3, maskDetectionSetting.scaleFactor);

        return normalised_roi;
    }

    return cv::Mat();
}
