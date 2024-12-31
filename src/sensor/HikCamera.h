#pragma once
#include "ImageSensor.h"
#include "HCNetSDK.h"

class HikCamera : public ImageSensor
{
public:
    HikCamera(int _queue_max_length, bool _is_full_drop, HWND _preview_handler);
    virtual cv::Mat getData() override;

private:
    virtual void dataCollectionLoop() override;
    void initDevice();
    void login();
    void releaseDevice();
    void startReplay();

    std::string device_ip;
    int device_port;
    std::string device_userName;
    std::string device_password;
    long userID;
    long replayHandler;
    HWND previewHandler;
};