#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <opencv2/opencv.hpp>

class Pipeline {
public:
    cv::Mat getCurrentImage();
    float getCurrentFPS();
};

#endif // PIPELINE_H_
