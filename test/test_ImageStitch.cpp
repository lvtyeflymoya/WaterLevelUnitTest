#include "test_ImageStitch.h"
#include "ImageSaver.h"
ImageSaver imageSaver("../results/test_ImageStitch");

ImageStitch::ImageStitch()
{
}

ImageStitch::~ImageStitch()
{
}

cv::Mat ImageStitch::getStitchedImage(const cv::Mat &img_target, const cv::Mat &img_reference)
{
    this->img_target = img_target;
    this->img_reference = img_reference;
    this->imageRegistration();
    return this->imageWarping();
}

void ImageStitch::imageRegistration()
{
    // 预处理
    cv::Mat img_target_gray;
    cv::Mat img_reference_gray;
    cv::cvtColor(this->img_target, img_target_gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(this->img_reference, img_reference_gray, cv::COLOR_BGR2GRAY);

    // 特征点检测
    cv::Ptr<cv::FeatureDetector> detector = cv::AKAZE::create();
    std::vector<cv::KeyPoint> keypoints_target, keypoints_reference;
    detector->detect(img_target_gray, keypoints_target);
    detector->detect(img_reference_gray, keypoints_reference);

    // 特征点描述
    cv::Ptr<cv::DescriptorExtractor> descriptor = cv::AKAZE::create();
    cv::Mat descriptors_target, descriptors_reference;
    descriptor->compute(img_target_gray, keypoints_target, descriptors_target);
    descriptor->compute(img_reference_gray, keypoints_reference, descriptors_reference);

    // 特征匹配
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors_target, descriptors_reference, matches);

    // 筛选匹配点
    std::vector<cv::Point2f> points_target, points_reference;
    for (size_t i = 0; i < matches.size(); i++)
    {
        if (matches[i].distance < 30)
        {
            points_target.push_back(keypoints_target[matches[i].queryIdx].pt);
            points_reference.push_back(keypoints_reference[matches[i].trainIdx].pt);
        }
    }

    // 计算单应变换矩阵
    this->H = cv::findHomography(points_target, points_reference, cv::RANSAC);
}

void ImageStitch::calculateWarpedCorners()
{
    cv::Size img_target_size = this->img_target.size();

    std::vector<cv::Point2f> img_target_corners = {
        cv::Point2f(0, 0),                                                 // 左上
        cv::Point2f(img_target_size.width - 1, 0),                         // 右上
        cv::Point2f(0, img_target_size.height - 1),                        // 左下
        cv::Point2f(img_target_size.width - 1, img_target_size.height - 1) // 右下
    };

    cv::perspectiveTransform(img_target_corners, this->warped_tartget_corners, this->H);
}

cv::Mat ImageStitch::imageWarping()
{
    // 计算目标图像的透视变换后四个角点的位置
    this->calculateWarpedCorners();

    // 计算拼接图像的大小
    cv::Size img_reference_size = this->img_reference.size();
    std::vector<cv::Point2f> img_reference_corners = {
        cv::Point2f(0, 0),
        cv::Point2f(img_reference_size.width - 1, 0),
        cv::Point2f(0, img_reference_size.height - 1),
        cv::Point2f(img_reference_size.width - 1, img_reference_size.height - 1)};

    // 合并参考图像和目标图像的角点，找到拼接图像的边界
    std::vector<cv::Point2f> all_corners = this->warped_tartget_corners;
    all_corners.insert(all_corners.end(), img_reference_corners.begin(), img_reference_corners.end());

    // 计算拼接图像的边界
    float min_x = std::min_element(all_corners.begin(), all_corners.end(), [](const cv::Point2f &a, const cv::Point2f &b)
                                   { return a.x < b.x; })
                      ->x;
    float min_y = std::min_element(all_corners.begin(), all_corners.end(), [](const cv::Point2f &a, const cv::Point2f &b)
                                   { return a.y < b.y; })
                      ->y;
    float max_x = std::max_element(all_corners.begin(), all_corners.end(), [](const cv::Point2f &a, const cv::Point2f &b)
                                   { return a.x < b.x; })
                      ->x;
    float max_y = std::max_element(all_corners.begin(), all_corners.end(), [](const cv::Point2f &a, const cv::Point2f &b)
                                   { return a.y < b.y; })
                      ->y;

    // 计算平移变换矩阵
    cv::Mat translation = cv::Mat::eye(3, 3, CV_64F);
    if (min_x < 0)
        translation.at<double>(0, 2) = -min_x;
    if (min_y < 0)
        translation.at<double>(1, 2) = -min_y;

    std::cout << "translation: \n"
              << translation << std::endl;

    // 变换target到reference坐标系下,并移动坐标系原点,使得最终的拼接图像的左上角为(0,0)
    cv::Mat warped_target;
    int result_width = static_cast<int>(std::floor(max_x - min_x) + 1);
    int result_height = static_cast<int>(std::floor(max_y - min_y) + 1);
    cv::warpPerspective(this->img_target, warped_target, translation * this->H, cv::Size(result_width, result_height));
    imageSaver.addImage(warped_target, "warped_target");

    // 将参考图像复制到拼接图像中
    cv::Mat result_image = warped_target.clone();
    int roi_x = static_cast<int>(translation.at<double>(0, 2));
    int roi_y = static_cast<int>(translation.at<double>(1, 2));
    int roi_width = img_reference_size.width;
    int roi_height = img_reference_size.height;

    cv::Mat roi(result_image, cv::Rect(roi_x, roi_y, roi_width, roi_height));
    this->img_reference.copyTo(roi);

    return result_image;
}

int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    cv::Mat img_reference = cv::imread("F:/MasterGraduate/02-ReferenceCode/01-VideoStitching/image/4.jpg");
    cv::Mat img_target = cv::imread("F:/MasterGraduate/02-ReferenceCode/01-VideoStitching/image/3.jpg");
    ImageStitch stitcher;
    cv::Mat stitched_image = stitcher.getStitchedImage(img_target, img_reference);
    imageSaver.addImage(stitched_image, "stitched_image");
}