#pragma once

#include <iostream>
#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

#include "opencv2/opencv.hpp"

#include "plog/Log.h"
#include "plog/Init.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include "plog/Formatters/TxtFormatter.h"

class ImageSensor
{
public:
    ImageSensor(int _queue_max_length, int _capture_interval_ms, bool _is_full_drop);
    ~ImageSensor();
    virtual void start();
    virtual void stop();
    void clear();
    void enqueueData(const cv::Mat& img);
    virtual cv::Mat getData() = 0;
protected:
    virtual void dataCollectionLoop() = 0;

    int queue_max_length;
    bool is_full_drop;
    std::atomic<bool> is_running;
    std::deque<cv::Mat> images;
    std::mutex mutex;
    std::condition_variable cv;
    std::thread sensor_thread;
    int capture_interval_ms;
};
