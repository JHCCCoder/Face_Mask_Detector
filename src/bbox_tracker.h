#ifndef BBOX_TRACKER_H_
#define BBOX_TRACKER_H_

#include <opencv2/opencv.hpp>
#include <vector>

#include "data_structures.h"

class BboxTracker {
public:
    BboxTracker(TrackerSetting&& setting);

    void update(cv::Point2f&& midpoint);
    const std::vector<TrackingObj> &getTrackingList() const;
private:
    uint nextId;
    TrackerSetting trackerSetting;
    std::vector<TrackingObj> trackingList;

    void registerObj(cv::Point2f&& midpoint);
    void deregisterObj(uint objectId);
};

#endif // BBOX_TRACKER_H_