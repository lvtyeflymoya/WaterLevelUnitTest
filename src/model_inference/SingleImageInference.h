// 测试单张图片推理
#include "SegmentationInference.h"
#include "../image_process/water_level_process_new.h"
#include "LocalImage.h"
#include <memory>

class SingleImageInference
{
public:
    SingleImageInference();
    ~SingleImageInference();

    // SegmentationInference *waterlevel_inference_rough = nullptr, *waterlevel_inference_fine = nullptr;

    void inference(const cv::Mat& input_image);
    void draw_waterlevel(cv::Mat& img, std::vector<double> line_equation);
    void save_image(const std::string& base_path);
    double calculateWaterLine();
    
private:
    cv::Mat original_image;   // 原始图像
    cv::Mat rough_result;     // 粗检测结果
    cv::Mat fine_result;      // 精检测结果
    cv::Mat waterlevel_image; // 带水位线图像
    bool rough_detected = false;    // 粗检测是否成功
    std::vector<double> waterLine_equation; // 水位线方程
    std::unique_ptr<SegmentationInference> waterlevel_inference_rough; 
    std::unique_ptr<SegmentationInference> waterlevel_inference_fine;  
    
}; 