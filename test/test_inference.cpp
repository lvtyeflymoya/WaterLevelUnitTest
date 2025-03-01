// 测试单张图片推理
#include "../src/model_inference/SegmentationInference.h"

// 创建一个类，读一张图片，执行推理
class SingleImageInference
{
public:
    SingleImageInference();
    ~SingleImageInference();

    SegmentationInference *water_inference_rough = nullptr, *water_inference_fine = nullptr;

    void inference_rough(cv::Mat &img);
    void inference_fine();
}; 

SingleImageInference::SingleImageInference()
{
    waterlevel_inference_rough = new SegmentationInference("F:/Works/2_WaterLine/Water_Level1/Model/rough_waternet.engine", 512, 512, 3); // 水位线粗检测
    waterlevel_inference_fine = new SegmentationInference("F:/Works/2_WaterLine/Water_Level1/Model/fine_waternet.engine", 2160, 256, 3);  // 水位线精检测
}
SingleImageInference::~SingleImageInference()
{
    delete waterlevel_inference_rough;
    delete waterlevel_inference_fine;
}

void SingleImageInference::inference_rough(std::string file_path)
{
    cv::Mat img = cv::imread(file_path)
        cv::Mat img_resize;
    cv::resize(img, img_resize, cv::Size(512, 512), cv::INTER_AREA);

    // 取图像对角线上的512个像素点，三通道值均相同时为红外夜间模式
    int num_night = 0;
    for (int i = 0; i < 512; i++)
    {
        if (img_resize.at<cv::Vec3b>(i, i)[0] == img_resize.at<cv::Vec3b>(i, i)[1] && img_resize.at<cv::Vec3b>(i, i)[0] == img_resize.at<cv::Vec3b>(i, i)[2])
        {
            num_night++;
        }
    }

    // 不是夜间模式时正常检测
    if (num_night != 512)
    {
        // 粗检测
        cv::Mat rough_output = water_inference_rough->do_inference(img_resize);
        cv::imshow("LocalImage", rough_output);
        cv::waitKey(100);
    }
}

void SingleImageInference::inference_fine()
{
}

int main()
{
    SingleImageInference signal_image_inference;
    signal_image_inference.inference_rough("D:/ImageAnnotation/chuanzha/Fabricate/003.jpg");
}