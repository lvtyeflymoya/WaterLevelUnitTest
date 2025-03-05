// 测试单张图片推理
#include "SingleImageInference.h"
#include "LocalImage.h"


int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    std::string image_path = "D:/ImageAnnotation/chuanzha/Fabricate/292.jpg";
    // LocalImage localImage("D:/ImageAnnotation/chuanzha/Fabricate/306.jpg",
    //                       1, false);
    // localImage.start();
    cv::Mat src_img = cv::imread(image_path);
    SingleImageInference signal_image_inference;
    signal_image_inference.inference(src_img);
    signal_image_inference.save_image("C:/Users/Zhang/Desktop/111/");
    double waterlinedata = signal_image_inference.calculateWaterLine();
    cout << waterlinedata << endl;
    return 0;
}