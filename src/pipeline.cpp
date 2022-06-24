#include <thread>

#include "pipeline.h"
#include "default_settings.cpp"

void PrintCallBack::callback(const TrackingObj& obj) {
    std::cout << "id: " << obj.id << obj.crossLineStatus << " passed. Mask status:" << obj.maskWearingType << std::endl;
}

Pipeline::Pipeline(bool* isRunning, EntryCheck::OnCrossCallBack* callBack)
        : isRunning(isRunning), cap(0),
          bboxTracker(Setting::defaultTrackerSetting),
          entryChecker(Setting::defaultCrossLineSetting, callBack),
          detector(Setting::defaultDetectorSetting)
{
    if (!cap.isOpened()) {
        throw std::runtime_error("Cannot access camera");
    }
    cv::Mat frame;
    cap.read(frame);
    if (frame.empty()) {
        fprintf(stderr, "Empty frame from camera\n");
        return;
    }
    currentResult = InferenceResult({frame});
}

cv::Mat Pipeline::getCurrentImage() {
    std::unique_lock<std::mutex> lock(mutex);
    cv::Mat image(currentResult.frame);
    lock.unlock();

    return image;
}

float Pipeline::getCurrentFPS() {
    return 10;
}

void Pipeline::setCrossLine(cv::Point2f pt1, cv::Point2f pt2) {
    entryChecker.setCrossLine(pt1, pt2);
}

void Pipeline::run() {
    while(*isRunning) {
        cv::Mat frame;
        cap >> frame;

        auto result = detector.process(frame);
        bboxTracker.update(result.faceList);
        auto& trackingObjs = bboxTracker.getTrackingList();

        entryChecker.drawCrossLine(result);
        for (auto& obj : trackingObjs) {
            entryChecker.checkCross(obj);
        }

        std::unique_lock<std::mutex> lock(mutex);
        currentResult = result;
        lock.unlock();
    }
}

SqlInsertCallback::SqlInsertCallback(){
    conn = new MysqlConn();
    // connect Mysql
    if (!conn->ConnectMysql()) {
        throw std::runtime_error("Cannot establish sql connection");
    }
}

SqlInsertCallback::~SqlInsertCallback(){
    conn->FreeConnect();
}

void SqlInsertCallback::callback(const TrackingObj &obj){
    int status = obj.maskWearingType;
    std::string query = "insert into userinfo(status) values("
                        +std::string("'")
                        +std::to_string(status)
                        +std::string("'")
                        +")";
    auto SQL = query.c_str();
    if(conn->InsertData(SQL) == 0)
        std::cout << "insert successful" << std::endl;
}

