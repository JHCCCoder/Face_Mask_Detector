/**
 * @file pipeline.h
 * @brief a pipeline class, inspired by https://github.com/ESE-Peasy/PosturePerfection
 *
 * @copyright Copyright (C) 2022 Minhao Han
 *
 */

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <opencv2/opencv.hpp>

#include "bbox_tracker.h"
#include "face_mask_detector.h"
#include "mysql_connection.h"

/**
 * @brief a simple implementation of OnCrossCallBack
 */
class PrintCallBack : public EntryCheck::OnCrossCallBack {
public:
    void callback(const TrackingObj& obj);
};

/**
 * @brief Pipeline class for this project, assemble all module together, pass data between all modules.
 */
class Pipeline {
public:
    explicit Pipeline(bool* isRunning, EntryCheck::OnCrossCallBack* callBack = nullptr);

    /**
     * @brief return the latest processed image, called by the cgi callback
     * @return the latest processed image
     */
    cv::Mat getCurrentImage();

    /**
     * @brief [Not implemented] return the frame per second of this processor
     * @return
     */
    float getCurrentFPS();

    /**
     * @brief set cross line, directly call the same name function in `entryChecker` object
     *
     * @param pt1 point 1
     * @param pt2 point 2
     */
    void setCrossLine(cv::Point2f pt1, cv::Point2f pt2);

    /**
     * @brief method to start the pipeline
     *
     * camera >> :frame: >> faceMaskDetector >> :inferenceResult: >> tracker >> :tracking object:
     * >> entryChecker;
     */
    void run();
private:
    cv::VideoCapture cap;
    FaceMaskDetector detector;
    BboxTracker bboxTracker;
    EntryCheck entryChecker;

    /**
     * @brief lock to prevent synchronous reading and writing to `currentResult` object
     */
    std::mutex mutex;

    InferenceResult currentResult;
    float fps;
    bool* isRunning;
};

class SqlInsertCallback : public EntryCheck::OnCrossCallBack{
private:
    MysqlConn * conn;
    void insertThread(const char* SQL);

public:
    SqlInsertCallback();
    ~SqlInsertCallback();
    void callback(const TrackingObj& obj) override;
};
#endif // PIPELINE_H_
