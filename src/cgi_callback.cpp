/*
 * Copyright (c) 2013-2021  Bernd Porr <mail@berndporr.me.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 */

#include <unistd.h>
#include <CppTimer.h>
#include <opencv2/opencv.hpp>
#include <json_fastcgi_web_api.h>

#include "base64.h"
#include "pipeline.h"
#include "cgi_callback.h"


JSONCgiGetCallback::JSONCgiGetCallback(Pipeline* pipelinePtr) {
    pipeline = pipelinePtr;
}

std::string JSONCgiGetCallback::getJSONString() {
    JSONCGIHandler::JSONGenerator jsonGenerator;
    std::vector<unsigned char> data_encode;

    cv::Mat image = pipeline->getCurrentImage();
    float fps = pipeline->getCurrentFPS();
    int res = imencode(".jpg", image, data_encode);
    std::string str_encode(data_encode.begin(), data_encode.end());
    const char* c = str_encode.c_str();
    // encoding images to base64 encode
    jsonGenerator.add("mat",base64_encode(c, str_encode.size()));
    jsonGenerator.add("fps", fps);
    return jsonGenerator.getJSON();
}

CVPOSTCallback::CVPOSTCallback(Pipeline* pipelinePtr) {
    pipeline = pipelinePtr;
}

void CVPOSTCallback::postString(std::string postArg) {
    auto m = JSONCGIHandler::postDecoder(postArg);
    float coordinate_x1 = atof(m["coordinate_x1"].c_str());
    float coordinate_x2 = atof(m["coordinate_x2"].c_str());
    float coordinate_y1 = atof(m["coordinate_y1"].c_str());
    float coordinate_y2 = atof(m["coordinate_y2"].c_str());
    pipeline->setCrossLine(cv::Point2f(coordinate_x1,coordinate_y1),cv::Point2f(coordinate_x2,coordinate_y2));
}
