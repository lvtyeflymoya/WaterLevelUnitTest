#include "ImageSaver.h"

int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    // 创建MatSaver对象，指定保存路径
    ImageSaver matSaver("../results");

    // 创建一些示例图片
    cv::Mat mat1 = cv::Mat::zeros(100, 100, CV_8UC3);
    cv::Mat mat2 = cv::Mat::ones(100, 100, CV_8UC3) * 255;

    // 添加图片到MatSaver
    matSaver.addImage(mat1, "image1.jpg");
    matSaver.addImage(mat2, "image2.jpg");
    mat2 = cv::Mat::ones(100, 100, CV_8UC3) * 128;
    matSaver.addImage(mat2, "image3.jpg");
}