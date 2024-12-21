#pragma once
#include <opencv2/opencv.hpp>
/**
 * @brief 不使用OpenCV的Image Stitching模块, 用基础算法实现拼接的功能
 */
class ImageStitch
{
public:
    ImageStitch();
    ~ImageStitch();
    
    cv::Mat getStitchedImage(const cv::Mat& img_target, const cv::Mat& img_reference);

private:
    void imageRegistration();
    void calculateWarpedCorners();
    cv::Mat imageWarping();

private:
    cv::Mat img_target, img_reference;
    cv::Mat H;
    std::vector<cv::Point2f> warped_tartget_corners;
};