#include <create_op_resolver.h>
#include <model.h>

#include "face_mask_detector.h"

#define COLOR_MASK cv::Scalar(100, 255, 0)
#define COLOR_NO_MASK cv::Scalar(0, 0, 255)
#define COLOR_WRONG_MASK cv::Scalar(5, 149, 252)

FaceMaskDetector::FaceMaskDetector(const std::string& modelPath, float confidenceThreshold)
                                   : confidenceThreshold(confidenceThreshold) {
    model = tflite::FlatBufferModel::BuildFromFile(modelPath.c_str());
    auto resolver = tflite::CreateOpResolver();
    tflite::InterpreterBuilder(*model, *resolver)(&interpreter);

    auto input = interpreter->inputs()[0];
    modelHeight = interpreter->tensor(input)->dims->data[1];
    modelWidth = interpreter->tensor(input)->dims->data[2];

    interpreter->AllocateTensors();
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(4);      // use multi-thread to boost inference process
}

InferenceResult FaceMaskDetector::process(cv::Mat &image) {
    auto cameraWidth = image.cols;
    auto cameraHeight = image.rows;

    auto inputTensor = interpreter->typed_tensor<float>(interpreter->inputs()[0]);
    preprocess(image, inputTensor);

    interpreter->Invoke();

    const float* detectionBboxes = interpreter->tensor(interpreter->outputs()[0])->data.f;
    const float* detectionClasses = interpreter->tensor(interpreter->outputs()[1])->data.f;
    const float* detectionConfidences = interpreter->tensor(interpreter->outputs()[2])->data.f;
    const int numDetections = int(*interpreter->tensor(interpreter->outputs()[3])->data.f);

    InferenceResult result;
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
                cv::rectangle(image, rect, COLOR_MASK, 2);
                cv::putText(image, "Wearing mask", cv::Point2f(x1, y1-5),
                            cv::FONT_HERSHEY_SIMPLEX, 0.7, COLOR_MASK, 1);
            }
            else if (type == 1) {
                face.maskWearingType = NO_MASK;
                cv::rectangle(image, rect, COLOR_NO_MASK, 2);
                cv::putText(image, "No mask", cv::Point2f(x1, y1-5),
                            cv::FONT_HERSHEY_SIMPLEX, 0.7, COLOR_NO_MASK, 1);
            }
            else if (type == 2) {
                face.maskWearingType = INCORRECT_WEARING;
                cv::rectangle(image, rect, COLOR_WRONG_MASK, 2);
                cv::putText(image, "Incorrect wearing", cv::Point2f(x1, y1-5),
                            cv::FONT_HERSHEY_SIMPLEX, 0.7, COLOR_WRONG_MASK, 1);
            }

            result.faceList.push_back(face);
        }
    }
    result.frame = std::move(image);

    return result;
}

void FaceMaskDetector::preprocess(const cv::Mat &image, float *out) const {
    cv::Mat tmp;
    cv::resize(image, tmp, cv::Size(modelWidth, modelHeight), cv::INTER_NEAREST);

    auto in = tmp.data;
    auto size = tmp.rows * tmp.cols * tmp.channels();
    for (int i = 0; i < size; i++) {
        out[i] = (float(in[i]) - 127.5f) / 127.5f;
    }
}
