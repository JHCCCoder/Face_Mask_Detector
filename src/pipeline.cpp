#include "pipeline.h"
#include "default_settings.cpp"

Pipeline::Pipeline(EntryCheck::OnCrossCallBack* callBack)
    : cap(0),
      bboxTracker(Setting::defaultTrackerSetting),
      entryChecker(Setting::defaultCrossLineSetting, callBack),
      detector()
      {

}

cv::Mat Pipeline::getCurrentImage() {
    std::unique_lock<std::mutex> lock(mutex);
    cv::Mat image(currentResult->frame);
    lock.unlock();

    return image;
}

float Pipeline::getCurrentFPS() {
    return fps;
}
