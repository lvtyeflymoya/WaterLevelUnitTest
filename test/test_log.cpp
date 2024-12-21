#include <iostream>
#include "opencv2/opencv.hpp"

int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    std::cout << "Hello, OpenCV!" << std::endl;
    cv::Mat img(512, 512, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::imshow("Image", img);
    cv::waitKey(0);
}