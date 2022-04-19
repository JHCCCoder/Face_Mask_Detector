#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <opencv2/opencv.hpp>

#include "bbox_tracker.h"
#include "face_mask_detector.h"
#include "mysql_connection.h"

class Pipeline {
public:
    explicit Pipeline(EntryCheck::OnCrossCallBack* callBack = nullptr);
    cv::Mat getCurrentImage();
    float getCurrentFPS();
private:
    cv::VideoCapture cap;
    FaceMaskDetector detector;
    BboxTracker bboxTracker;
    EntryCheck entryChecker;

    std::mutex mutex;

    InferenceResult* currentResult;
    float fps;
};

#endif // PIPELINE_H_
