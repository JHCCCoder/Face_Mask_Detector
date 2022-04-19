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

/**
 * Callback handler which returns data to the
 * nginx server. Here, simply the current temperature
 * and the timestamp is transmitted to nginx and the
 * javascript application.
 **/
class JSONCgiGetCallback : public JSONCGIHandler::GETCallback {
private:
    /**
     * Pointer to the ADC event handler because it keeps
     * the data in this case. In a proper application
     * that would be probably a database class or a
     * controller keeping it all together.
     **/
    Pipeline* pipeline;

public:
    /**
     * Constructor: argument is the cvImage callback handler
     * which keeps the data as a simple example.
     **/
    JSONCgiGetCallback(Pipeline* pipelinePtr) {
        pipeline = pipelinePtr;
    }

    /**
     * Gets the data sends it to the webserver.
     * The callback creates two json entries.
     **/
    std::string getJSONString() {
        JSONCGIHandler::JSONGenerator jsonGenerator;
        std::vector<unsigned char> data_encode;

        cv::Mat image = pipeline->getCurrentImage();
        float fps = pipeline->getCurrentFPS();
        int res = imencode(".jpg", image, data_encode);
        std::string str_encode(data_encode.begin(), data_encode.end());
        const char* c = str_encode.c_str();

        jsonGenerator.add("mat",base64_encode(c, str_encode.size()));
        jsonGenerator.add("fps", fps);
        return jsonGenerator.getJSON();
    }
};


/**
 * Callback handler which receives the JSON from jQuery
 **/
class CVPOSTCallback : public JSONCGIHandler::POSTCallback {
public:
    CVPOSTCallback() {}

    /**
     * receives the JSON from jQuery.
     **/
    virtual void postString(std::string postArg) {}
};
