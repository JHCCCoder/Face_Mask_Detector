#include <create_op_resolver.h>
#include <model.h>

#include "face_mask_detector.h"

#define COLOR_MASK cv::Scalar(100, 255, 0)
#define COLOR_NO_MASK cv::Scalar(255, 0, 0)
#define COLOR_WRONG_MASK cv::Scalar(252, 149, 5)


FaceMaskDetector::FaceMaskDetector(const std::string& modelPath, int modelWidth, int modelHeight,
                                   int modelChannels, float confidenceThreshold)
                                   : modelWidth(modelWidth),
                                   modelHeight(modelHeight),
                                   modelChannels(modelChannels),
                                   confidenceThreshold(confidenceThreshold) {
    model = tflite::FlatBufferModel::BuildFromFile(modelPath.c_str());
    auto resolver = tflite::CreateOpResolver();
    tflite::InterpreterBuilder(*model, *resolver)(&interpreter);

    interpreter->AllocateTensors();
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(4);      // use multi-thread to boost inference process
}

InferenceResult FaceMaskDetector::process(cv::Mat &image) {
    auto cameraWidth = image.cols;
    auto cameraHeight = image.rows;

    auto inputTensor = interpreter->typed_tensor<float>(interpreter->inputs()[0]);
    preprocess(image, inputTensor);

    const float* detectionBboxes = interpreter->tensor(interpreter->outputs()[0])->data.f;
    const float* detectionClasses = interpreter->tensor(interpreter->outputs()[1])->data.f;
    const float* detectionConfidences = interpreter->tensor(interpreter->outputs()[2])->data.f;
    const int numDetections = int(*interpreter->tensor(interpreter->outputs()[3])->data.f);

    InferenceResult result({std::move(image)});
    for (int i = 0; i < numDetections; i++) {
        if (detectionConfidences[i] > confidenceThreshold) {
            int type = int(detectionClasses[i]);
            float y1 = detectionBboxes[4*i] * float(cameraHeight);
            float x1 = detectionBboxes[4*i + 1] * float(cameraWidth);
            float y2 = detectionBboxes[4*i + 2] * float(cameraHeight);
            float x2 = detectionBboxes[4*i + 3] * float(cameraWidth);

            cv::Point2f topLeft(x1, y1);
            cv::Point2f bottomRight(x2, y2);

            FaceInfo face;
            face.topLeft = topLeft;
            face.bottomRight = bottomRight;
            face.confidence = detectionConfidences[i];

            // draw rectangle on face covering area
            cv::Rect2f rect(topLeft, bottomRight);
            if (type == 0) {
                face.maskWearingType = WEARING_MASK;
                cv::rectangle(result.frame, rect, COLOR_MASK, 2);
                cv::putText(result.frame, "Wearing mask", cv::Point2f(x1, y1-5),
                            cv::FONT_HERSHEY_SIMPLEX, 0.7, COLOR_MASK, 1);
            }
            else if (type == 1) {
                face.maskWearingType = NO_MASK;
                cv::rectangle(result.frame, rect, COLOR_NO_MASK, 2);
                cv::putText(result.frame, "No mask", cv::Point2f(x1, y1-5),
                            cv::FONT_HERSHEY_SIMPLEX, 0.7, COLOR_NO_MASK, 1);
            }
            else if (type == 2) {
                face.maskWearingType = INCORRECT_WEARING;
                cv::rectangle(result.frame, rect, COLOR_WRONG_MASK, 2);
                cv::putText(result.frame, "Incorrect wearing", cv::Point2f(x1, y1-5),
                            cv::FONT_HERSHEY_SIMPLEX, 0.7, COLOR_WRONG_MASK, 1);
            }

            result.faceList.push_back(face);
        }
    }

    return result;
}

void FaceMaskDetector::preprocess(cv::Mat &image, float *out) const {
    cv::Mat tmp;
    cv::resize(image, tmp, cv::Size(modelWidth, modelHeight), cv::INTER_NEAREST);

    auto in = tmp.data;
    auto size = tmp.rows * tmp.cols * tmp.channels();
    for (int i = 0; i < size; i++) {
        out[i] = (float(in[i]) - 127.5f) / 127.5f;
    }
}
