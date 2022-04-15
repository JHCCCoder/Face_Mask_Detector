#include "bbox_tracker.h"

BboxTracker::BboxTracker(TrackerSetting &&setting)
    : trackerSetting(setting) {
    nextId = 0;
}

void BboxTracker::update(cv::Point2f&& midpoint) {

}

void BboxTracker::registerObj(cv::Point2f&& midpoint) {
    TrackingObj obj({nextId++, midpoint, 0});
    this->trackingList.push_back(obj);
}

void BboxTracker::deregisterObj(uint objectId) {
    std::remove_if(this->trackingList.begin(), this->trackingList.end(),
                   [objectId](const TrackingObj& obj) { return obj.id == objectId; });
}

const std::vector<TrackingObj> &BboxTracker::getTrackingList() const {
    return trackingList;
}
