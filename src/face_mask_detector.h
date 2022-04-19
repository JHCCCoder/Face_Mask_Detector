#ifndef FACE_DETECTOR_H_
#define FACE_DETECTOR_H_

#include <opencv2/opencv.hpp>
#include <tensorflow/lite/interpreter.h>
#include <model_builder.h>

#include "data_structures.h"

class FaceMaskDetector {
public:
    FaceMaskDetector(const std::string& modelPath, float confidenceThreshold);
    InferenceResult process(cv::Mat& image);
private:
    std::unique_ptr<tflite::Interpreter> interpreter;
    std::unique_ptr<tflite::FlatBufferModel> model;
    int modelWidth;
    int modelHeight;
    float confidenceThreshold;

    void preprocess(const cv::Mat& image, float* out) const;
};

#endif // FACE_DETECTOR_H_