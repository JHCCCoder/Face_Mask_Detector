/**
 * @file pipeline.h
 * @brief a pipeline class, inspired by https://github.com/ESE-Peasy/PosturePerfection
 *
 * @copyright Copyright (C) 2022
 *
 */

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <opencv2/opencv.hpp>

#include "bbox_tracker.h"
#include "face_mask_detector.h"

class PrintCallBack : public EntryCheck::OnCrossCallBack {
public:
    void callback(const TrackingObj& obj);
};

class Pipeline {
public:
    explicit Pipeline(bool* isRunning, EntryCheck::OnCrossCallBack* callBack = nullptr);

    cv::Mat getCurrentImage();
    float getCurrentFPS();
    void setCrossLine(cv::Point2f pt1, cv::Point2f pt2);

    void run();
private:
    cv::VideoCapture cap;
    FaceMaskDetector detector;
    BboxTracker bboxTracker;
    EntryCheck entryChecker;

    std::mutex mutex;

    InferenceResult currentResult;
    float fps;
    bool* isRunning;
};

#endif // PIPELINE_H_
