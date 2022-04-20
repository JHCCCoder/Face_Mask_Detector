/**
 * @file bbox_tracker.h
 * @brief Tracker based on the movement of bounding boxes, and to
 * check whether the tracked object cross the preset line.
 *
 * @copyright Copyright (C) 2022  Minhao Han
 *
 */

#ifndef BBOX_TRACKER_H_
#define BBOX_TRACKER_H_

#include <opencv2/opencv.hpp>
#include <vector>

#include "data_structures.h"


class BboxTracker {
public:
    explicit BboxTracker(TrackerSetting& setting);

    /**
     * @brief use a list of faces to update tracking objects
     *
     * take a list of bounding boxes of faces, calculate the midpoint of a bounding box
     * then assign every point the its nearest tracking object. If some points or tracking objects
     * do not match any other object, it will be taken as a New-appear object or a Disappearing object.
     *
     * @param faces a list of bounding boxes of faces
     */
    void update(const std::vector<FaceInfo>& faces);

    /**
     * @brief: return the reference of the list of all current tracking objects
     *
     * @return the reference of the list of all current tracking objects
     */
    std::vector<TrackingObj> &getTrackingList();
private:
    /**
     * @brief id of next tracking object
     */
    uint nextId;

    /**
     * @brief setting instance to control how many frames a tracking object
     * disappears from the screen will be considered as a Disappear object.
     */
    TrackerSetting trackerSetting;

    /**
     * @brief list to record all current tracking objects
     */
    std::vector<TrackingObj> trackingList;

    /**
     * @brief register a new point as a tracking object
     *
     * register a new point into the tracking object list, make the nextId counter increase by 1
     *
     * @param midpoint the midpoint of the corresponding bounding box
     */
    void registerObj(cv::Point2f midpoint);

    /**
     * @brief deregister a tracking object from tracking list
     *
     * @param objectId object id of tracking object
     */
    void deregisterObj(uint objectId);

    /**
     * @brief helper function to calculate the distance between two points
     *
     * @param pt1 point1
     * @param pt2 point2
     * @return the distance between two points
     */
    static double calcDistance(const cv::Point2f& pt1, const cv::Point2f& pt2);

    /**
     * @brief helper function to calculate the midpoint of a bounding box (marked by two points)
     *
     * @param pt1 point 1
     * @param pt2 point 2
     * @return the midpoint of the two points
     */
    static cv::Point2f getMidpoint2f(const cv::Point2f& pt1, const cv::Point2f& pt2);
    static cv::Point2f getMidpoint2f(float x1, float x2, float y1, float y2);
};

/**
 * @brief intermediate data structure to record the distance between a tracking object
 * and a point.
 */
struct PointDistance {
    /**
     * @brief pointer to tracking object
     */
    TrackingObj* src;

    /**
     * @brief pointer to the target point
     */
    cv::Point2f* target;

    /**
     * @brief the distance between tracking object and target point
     */
    double distance;
};


/**
 * @brief a class to handle whether a tracking object has crossed the entrance line
 * and trigger the callback function if so
 */
class EntryCheck {
public:
    /**
     * @brief Callback class to handle the senario that someone crosses the set entrance line
     */
    class OnCrossCallBack {
    public:

        /**
         * @brief callback function triggered when some one crosses the entrance line
         * @param obj the tracking object that crosses the line
         */
        virtual void callback(const TrackingObj& obj) = 0;
    };

    explicit EntryCheck(CrossLineSetting& setting, OnCrossCallBack* callback = nullptr);

    /**
     * @brief draw current entrance line on the image
     *
     * @param image processed image
     */
    void drawCrossLine(InferenceResult& image);

    /**
     * @brief check if the object cross the entrance line, if true, trigger callback function
     *
     * make the current point and the start point of a tracking object a line segment,
     * if the line segment intersect with the entrance line, it is considered as one time of entrance.
     * the crossStatus will be set as true to prevent multi-count within a short time.
     *
     * @param obj the tracking object to check
     */
    void checkCross(TrackingObj& obj);

    /**
     * @brief set the cross line
     *
     * @param pt1 point 1
     * @param pt2 point 2
     */
    void setCrossLine(cv::Point2f pt1, cv::Point2f pt2);
private:
    /**
     * @brief a helper function to determine whether two line segments intersect
     *
     * @param linePt1 point 1 of line A
     * @param linePt2 point 2 of line A
     * @param targetPt one point of line B
     * @return
     */
    static float direction(const cv::Point2f& linePt1, const cv::Point2f& linePt2,
                    const cv::Point2f& targetPt);

    CrossLineSetting crossLineSetting;

    /**
     * @brief callback class pointer
     */
    OnCrossCallBack* onCrossCallBack = nullptr;
};


#endif // BBOX_TRACKER_H_