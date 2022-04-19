#include "face_mask_detector.h"


FaceMaskDetector::FaceMaskDetector(const std::string& modelPath) {
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("ssd_mobilenet_v2_fpnlite.tflite");
}