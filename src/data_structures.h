#ifndef FACE_MASK_DETECTOR_DATA_STRUCTURES_H_
#define FACE_MASK_DETECTOR_DATA_STRUCTURES_H_

#include <opencv2/opencv.hpp>
#include <vector>

enum MaskWearingType {
    WEARING_MASK = 0,
    NO_MASK = 1,
    INCORRECT_WEARING = 2
};

struct DetectorSetting {
    std::string modelPath;
    float confidenceThreshold;
};

struct FaceInfo {
    cv::Point2f topLeft;
    cv::Point2f bottomRight;

    float confidence;
    MaskWearingType maskWearingType;
};

struct InferenceResult {
    cv::Mat frame;
    std::vector<FaceInfo> faceList;
};

struct TrackingObj {
    uint id;
    cv::Point2f curMidpoint;
    cv::Point2f startPoint;
    int disappearCnt;
    bool crossLineStatus;
};

struct TrackerSetting {
    int frameToDisappear;
};

struct CrossLineSetting {
    cv::Point2f pt1;
    cv::Point2f pt2;
    cv::Scalar color;
    int thickness;
};

#endif //FACE_MASK_DETECTOR_DATA_STRUCTURES_H_
