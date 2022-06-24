/**
 * @file cgi_callback.h
 * @brief cgi callback class definition
 *
 * @copyright Copyright (C) 2022  Xiaoyang Wang
 *
 */

#ifndef FACE_MASK_DETECTOR_CGI_CALLBACK_H
#define FACE_MASK_DETECTOR_CGI_CALLBACK_H

#include <unistd.h>
#include <CppTimer.h>
#include <opencv2/opencv.hpp>
#include <json_fastcgi_web_api.h>

#include "base64.h"
#include "pipeline.h"

/**
 * @brief Callback handler which returns data to the
 * nginx server.The current cv image
 * is transmitted to nginx.
 **/
class JSONCgiGetCallback : public JSONCGIHandler::GETCallback {
private:
    Pipeline* pipeline;

public:
    /**
     * @brief Constructor: argument is the cvImage callback handler
     * which keeps the data as a simple example.
     **/
    JSONCgiGetCallback(Pipeline* pipelinePtr);

    /**
     * @brief Gets the data sends it to the webserver.
     * The callback creates two json entries.
     **/
    std::string getJSONString();
};

/**
 * @brief Callback handler which receives the JSON from jQuery
 **/
class CVPOSTCallback : public JSONCGIHandler::POSTCallback {
private:
    Pipeline* pipeline;

public:
    CVPOSTCallback(Pipeline* pipelinePtr);

    /**
     * @brief receives the JSON from jQuery.
     **/
    void postString(std::string postArg);
};
#endif //FACE_MASK_DETECTOR_CGI_CALLBACK_H
