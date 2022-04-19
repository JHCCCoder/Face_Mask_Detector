#ifndef FACE_MASK_DETECTOR_DATA_STRUCTURES_H_
#define FACE_MASK_DETECTOR_DATA_STRUCTURES_H_

#include <opencv2/opencv.hpp>
#include <vector>

struct FaceInfo {
    // bounding box coordinates
    float x1;
    float y1;
    float x2;
    float y2;

    // todo: refactor coordinates with cv::Point
    cv::Point2f topLeft;
    cv::Point2f bottomRight;

    // face confidence faceScore
    float faceScore;

    // mask confidence faceScore
    float maskScore;
    bool isWearingMask;
};

struct Image {
    cv::Mat frame;
    std::vector<FaceInfo> faceList;
};

struct MaskDetectorSetting {
    int normalisedWidth;
    int normalisedHeight;
    float scaleFactor;
    float scoreThreshold;
    std::string modelPath;

    inline int imageSize() const { return normalisedHeight * normalisedWidth; }
};

struct FaceDetectorSetting {
    float confidenceThreshold;
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
