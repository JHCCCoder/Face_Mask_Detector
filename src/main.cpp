#include "pipeline.h"
#include "cgi_callback.cpp"

int main() {
    bool isRunning = true;
    bool* isRunningPtr = &isRunning;

    PrintCallBack callback;
    Pipeline pipeline(isRunningPtr, &callback);


    JSONCgiGetCallback cgiGetCallback(&pipeline);
    CVPOSTCallback postCallback;
    JSONCGIHandler* fastCGIHandler = new JSONCGIHandler(&cgiGetCallback,
                                                        &postCallback,
                                                        "/tmp/sensorsocket");

    pipeline.run();

    isRunning = false;
    return 0;
}