#include "pipeline.h"

Pipeline::Pipeline() {

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
