#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <opencv2/opencv.hpp>

#include "bbox_tracker.h"
#include "face_mask_detector.h"
#include "mysql_connection.h"

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

class CrossCallBack :public EntryCheck::OnCrossCallBack{
private:
    MysqlConn * conn;

public:
    CrossCallBack();
    ~CrossCallBack();
    virtual void callback(const TrackingObj& obj){
        const char *SQL ="";
        int status = obj.maskWearingType;
        std::string query = "insert into userinfo(status) values("
                +std::string("'")
                +std::to_string(status)
                +std::string("'")
                +")";
        SQL =  query.c_str();
        if(conn->InsertData(SQL) == 0)
            printf("insert successful \n");
    }
};
#endif // PIPELINE_H_
