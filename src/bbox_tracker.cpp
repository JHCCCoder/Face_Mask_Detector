#include <set>

#include "bbox_tracker.h"

BboxTracker::BboxTracker(TrackerSetting &&setting)
    : trackerSetting(setting) {
    nextId = 0;
}

void BboxTracker::update(const std::vector<FaceInfo>& faceRects) {
    if (faceRects.empty()) {
        for (auto& obj : trackingList) {
            obj.disappearCnt++;

            if (obj.disappearCnt > trackerSetting.frameToDisappear) {
                deregisterObj(obj.id);
            }
        }
        return;
    }

    // initialize mid point list
    std::vector<cv::Point2f> inputMidPoints;
    inputMidPoints.reserve(faceRects.size());
    for (auto& rect : faceRects) {
        inputMidPoints.push_back(getMidpoint2f(rect.x1, rect.x2, rect.y1, rect.y2));
    }

    // if no tracking object is recorded, register all points as tracking objects
    if (trackingList.empty()) {
        for (auto& point : inputMidPoints) {
            registerObj(&point);
        }
    }

    // otherwise, try to match mid-points to existing tracking objects
    else {
        // add distances between all points to a vector
        std::vector<PointDistance> pointDistances;

        for (auto& obj : trackingList) {
            for (auto& point : inputMidPoints) {
                pointDistances.push_back({
                    &obj, &point, calcDistance(obj.midpoint, point)
                });
            }
        }

        // init sets
        std::vector<TrackingObj*> unusedTrackers;
        std::vector<cv::Point2f*> unusedMidPoints;
        unusedTrackers.reserve(trackingList.size());
        unusedMidPoints.reserve(inputMidPoints.size());
        for (auto& tracker : trackingList) { unusedTrackers.push_back(&tracker); }
        for (auto& point : inputMidPoints) { unusedMidPoints.push_back(&point); }

        // for every loop, mark the closest point pairs as used,
        // and remove all relevant pointDistance object
        // loop until no object left in vector
        while (!pointDistances.empty()) {
            auto minPd = std::min_element(pointDistances.begin(), pointDistances.end(), [](const PointDistance& lpd, const PointDistance& rpd) {
                return lpd.distance < rpd.distance;
            });

            unusedTrackers.erase(std::remove_if(unusedTrackers.begin(), unusedTrackers.end(),
                           [minPd](TrackingObj* obj) {
                return minPd->src->id == obj->id;
            }), unusedTrackers.end());
            unusedMidPoints.erase(std::remove_if(unusedMidPoints.begin(), unusedMidPoints.end(),
                           [minPd](cv::Point2f* point) {
                return *point == *minPd->target;
            }), unusedMidPoints.end());
            pointDistances.erase(std::remove_if(pointDistances.begin(), pointDistances.end(),
                           [minPd](const PointDistance& pd) {
                return *pd.target == *(minPd->target) || pd.src->id == minPd->src->id;
            }), pointDistances.end());

            // update tracking object coordinates
            minPd->src->midpoint = *minPd->target;
        }

        // in the event that the number of tracker objects is greater than
        // the number of midpoints, some objects could potentially disappear
        if (unusedTrackers.size() > unusedMidPoints.size()) {
            for (auto tracker : unusedTrackers) {
                tracker->disappearCnt++;
                if (tracker->disappearCnt > trackerSetting.frameToDisappear) {
                    deregisterObj(tracker->id);
                }
            }
        }
        // else some new object could potentially appear in camera
        // register this object as a new tracking object
        else {
            for (auto point : unusedMidPoints) {
                registerObj(point);
            }
        }
    }
}

void BboxTracker::registerObj(cv::Point2f* midpoint) {
    TrackingObj obj({nextId++, *midpoint, 0});
    this->trackingList.push_back(obj);
}

void BboxTracker::deregisterObj(uint objectId) {
    trackingList.erase(std::remove_if(this->trackingList.begin(), this->trackingList.end(),
                   [objectId](const TrackingObj& obj) { return obj.id == objectId; }), trackingList.end());
}

const std::vector<TrackingObj> &BboxTracker::getTrackingList() const {
    return trackingList;
}

double BboxTracker::calcDistance(const cv::Point2f& pt1, const cv::Point2f& pt2) {
    return cv::norm(pt1 - pt2);
}

cv::Point2f BboxTracker::getMidpoint2f(const cv::Point2f &pt1, const cv::Point2f &pt2) {
    return (pt1 + pt2) * .5;
}

cv::Point2f BboxTracker::getMidpoint2f(float x1, float x2, float y1, float y2) {
    return {(x1 + x2) / 2, (y1 + y2) / 2};
}
