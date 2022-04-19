#define BOOST_TEST_MODULE BboxTrackerTest

#include <boost/test/unit_test.hpp>

#include "bbox_tracker.h"


BOOST_AUTO_TEST_CASE(IdleTest) {
    TrackerSetting setting({3});
    BboxTracker tracker(std::move(setting));
    std::vector<std::vector<FaceInfo>> testSources({
           {},
           {},
           {},
           {},
           {}
    });

    int i = 0;
    for (; i < testSources.size(); i++) {
        tracker.update(testSources[i]);
        auto trackingList = tracker.getTrackingList();

        BOOST_CHECK_EQUAL(trackingList.size(), 0);
    }
}

BOOST_AUTO_TEST_CASE(TrackingUpdateTest) {
    TrackerSetting setting({3});
    BboxTracker tracker(std::move(setting));
    std::vector<std::vector<FaceInfo>> testSources({
        {{1, 1, 1, 1}},
        {{2, 2, 2, 2}},
        {{3, 3, 3, 3}}
    });

    for (const auto& faces : testSources) {
        tracker.update(faces);
        auto trackingList = tracker.getTrackingList();

        BOOST_CHECK_EQUAL(trackingList.size(), 1);
    }

    BOOST_CHECK_EQUAL(tracker.getTrackingList()[0].curMidpoint.x, 3);
    BOOST_CHECK_EQUAL(tracker.getTrackingList()[0].curMidpoint.y, 3);
}

BOOST_AUTO_TEST_CASE(NewAppearTest) {
    TrackerSetting setting({3});
    BboxTracker tracker(std::move(setting));
    std::vector<std::vector<FaceInfo>> testSources({
           {{1, 1, 1, 1}},
           {{2, 2, 2, 2}},
           {{3, 3, 3, 3}},
           {{3,3,3,4}, {1,1,1,1}}
    });

    int i = 0;
    for (; i < 3; i++) {
        tracker.update(testSources[i]);
        auto trackingList = tracker.getTrackingList();

        BOOST_CHECK_EQUAL(trackingList.size(), 1);
    }

    tracker.update(testSources[i]);
    auto trackingList = tracker.getTrackingList();
    BOOST_CHECK_EQUAL(trackingList.size(), 2);
}

BOOST_AUTO_TEST_CASE(DisappearTest) {
    TrackerSetting setting({3});
    BboxTracker tracker(std::move(setting));
    std::vector<std::vector<FaceInfo>> testSources({
           {{1, 1, 1, 1}},
           {{2, 2, 2, 2}},
           {{3, 3, 3, 3}},
           {},
           {},
           {},
           {}
    });

    int i = 0;
    for (; i < 3; i++) {
        tracker.update(testSources[i]);
    }
    auto trackingList = tracker.getTrackingList();
    BOOST_CHECK_EQUAL(trackingList.size(), 1);

    for (; i < testSources.size(); i++) {
        tracker.update(testSources[i]);
    }
    trackingList = tracker.getTrackingList();
    BOOST_CHECK_EQUAL(trackingList.size(), 0);
}

BOOST_AUTO_TEST_CASE(CheckCrossLineTest) {
    TrackingObj obj({0, {5, 5}, {0, 0}, 0, false});
    CrossLineSetting setting({
         {0, 3},
         {3, 0},
         {123, 255, 0},
         2
    });
    EntryCheck checker(std::move(setting));

    checker.checkCross(obj);

    BOOST_CHECK_EQUAL(obj.crossLineStatus, true);
}

BOOST_AUTO_TEST_CASE(CheckCrossLineFailTest) {
    TrackingObj obj({0, {1, 1}, {0, 0}, 0, false});
    CrossLineSetting setting({
                                     {0, 3},
                                     {3, 0},
                                     {123, 255, 0},
                                     2
                             });
    EntryCheck checker(std::move(setting));

    checker.checkCross(obj);

    BOOST_CHECK_EQUAL(obj.crossLineStatus, false);
}