#include "ImageSensor.h"

ImageSensor::ImageSensor(int _queue_max_length, int _capture_interval_ms, bool _is_full_drop)
    : queue_max_length(_queue_max_length),
      is_full_drop(_is_full_drop),
      is_running(false),
      capture_interval_ms(_capture_interval_ms)
{
    static plog::ColorConsoleAppender<plog::MyFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);
}

ImageSensor::~ImageSensor()
{
    this->stop();
}

void ImageSensor::start()
{
    if (this->is_running.load() == false)
    {
        is_running.store(true);
        PLOGI << "ImageSensor Start!";
        sensor_thread = std::thread(&ImageSensor::dataCollectionLoop, this);
    }
}

void ImageSensor::stop()
{
    if (this->is_running.load() == false)
        return;

    this->is_running.store(false);
    cv.notify_all();
    if (this->sensor_thread.joinable())
        sensor_thread.join();
    PLOGI << "ImageSensor Stop!";
}

void ImageSensor::clear()
{
    std::unique_lock<std::mutex> lock(mutex);
    while (!images.empty())
    {
        images.pop_front();
    }
}

void ImageSensor::enqueueData(const cv::Mat &img)
{
    std::unique_lock<std::mutex> lock(mutex);
    if (images.size() >= queue_max_length)
    {
        if (this->is_full_drop == true)
        {
            PLOGV << "Drop latest image because queue is full!";
            return;
        }
        else
        {
            PLOGV << "Drop oldest image because queue is full!";
            images.pop_front();
        }
    }
    images.push_back(img.clone());
    cv.notify_one();
    PLOGV << "enqueueData! queue size: " << this->images.size();
}