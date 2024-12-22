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

    cv::Mat getStitchedImage(const cv::Mat &img_target, const cv::Mat &img_reference);

private:
    void imageRegistration();
    void calculateWarpedCorners();
    cv::Mat imageWarping();
    cv::Mat imageBlending(const cv::Mat &warped_target);

private:
    cv::Mat img_target, img_reference;
    cv::Mat H;           // 将img_target变换到img_reference的坐标系
    cv::Mat translation; // 用于对img_reference的坐标系进行平移,以确保warped_tartget的所有角点的坐标都大于等于0
    std::vector<cv::Point2f> warped_tartget_corners;
};