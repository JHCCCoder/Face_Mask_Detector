/**
 * @file face_mask_detector.h
 * @brief inference core class, inspired and modified from
 * https://github.com/Qengineering/TensorFlow_Lite_Face_Mask_RPi_64-bits.git
 *
 * @copyright Copyright (C) 2022  Minhao Han
 *
 */

#ifndef FACE_DETECTOR_H_
#define FACE_DETECTOR_H_

#include <opencv2/opencv.hpp>
#include <tensorflow/lite/interpreter.h>
#include <model_builder.h>

#include "data_structures.h"

/**
 * @brief Inference core to detect whether people in the image is wearing mask or not.
 */
class FaceMaskDetector {
public:
    FaceMaskDetector(DetectorSetting& setting);

    /**
     * @brief process the image with tensor flow model
     *
     * @param image image to process
     * @return the inference result which contains face bounding boxes, confidence and wearingStatus
     */
    InferenceResult process(cv::Mat& image);
private:
    std::unique_ptr<tflite::Interpreter> interpreter;
    std::unique_ptr<tflite::FlatBufferModel> model;
    int modelWidth;
    int modelHeight;
    /**
     * @brief confidence threshold to determine whether a bounding box is considered as a face.
     */
    float confidenceThreshold;

    /**
     * @brief preprocess the source image and put the preprocessed image into the input tensor
     *
     * first resize the source image to the size required by the model, and then put the preprocessed
     * image into the input tensor for the interpreter to process.
     *
     * @param image image to process
     * @param out input tensor as the output of the function
     */
    void preprocess(const cv::Mat& image, float* out) const;
};

#endif // FACE_DETECTOR_H_