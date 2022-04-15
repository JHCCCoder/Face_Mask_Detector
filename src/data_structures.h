#ifndef FACE_MASK_DETECTOR_DATA_STRUCTURES_H_
#define FACE_MASK_DETECTOR_DATA_STRUCTURES_H_

struct FaceInfo {
    // bounding box coordinates
    float x1;
    float y1;
    float x2;
    float y2;

    // credibility score
    float score;
};

struct Image {

};

struct TrackingObj {
    uint id;
    cv::Point2f midpoint;
    int disappearCnt;
};

struct TrackerSetting {
    int frameToDisappear;
};

#endif //FACE_MASK_DETECTOR_DATA_STRUCTURES_H_
