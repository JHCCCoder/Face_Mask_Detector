#include "data_structures.h"

#define COLOR_DEFAULT_CROSS_LINE cv::Scalar(0, 0, 0)

namespace Setting {
    TrackerSetting defaultTrackerSetting({10});

    CrossLineSetting defaultCrossLineSetting({
        {0, 0},
        {0, 0},
        {COLOR_DEFAULT_CROSS_LINE},
        2
    });

    DetectorSetting defaultDetectorSetting({
        "assets/ssdlite_mobilenet_v2.tflite",
        0.6f
    });
}
