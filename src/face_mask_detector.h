#ifndef FACE_DETECTOR_H_
#define FACE_DETECTOR_H_

#include <opencv2/opencv.hpp>
#include <tensorflow/lite/interpreter.h>

class FaceMaskDetector {
public:
    FaceMaskDetector(const std::string& modelPath);
    void process(cv::Mat& image);
private:
    std::unique_ptr<tflite::Interpreter> interpreter;
    void SetInputImage(cv::Mat& image, float* out);
};

#endif // FACE_DETECTOR_H_