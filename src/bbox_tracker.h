#ifndef BBOX_TRACKER_H_
#define BBOX_TRACKER_H_

#include <opencv2/opencv.hpp>
#include <vector>

#include "data_structures.h"

class BboxTracker {
public:
    BboxTracker(TrackerSetting&& setting);

    void update(const std::vector<FaceInfo>& point);
    const std::vector<TrackingObj> &getTrackingList() const;
private:
    uint nextId;
    TrackerSetting trackerSetting;
    std::vector<TrackingObj> trackingList;

    void registerObj(cv::Point2f* midpoint);
    void deregisterObj(uint objectId);
    static double calcDistance(const cv::Point2f& pt1, const cv::Point2f& pt2);
    static cv::Point2f getMidpoint2f(const cv::Point2f& pt1, const cv::Point2f& pt2);
    static cv::Point2f getMidpoint2f(float x1, float x2, float y1, float y2);
};

struct PointDistance {
    TrackingObj* src;
    cv::Point2f* target;
    double distance;
};

#endif // BBOX_TRACKER_H_