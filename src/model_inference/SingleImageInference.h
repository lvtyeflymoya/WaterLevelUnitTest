// 测试单张图片推理
#include "SegmentationInference.h"
#include "../image_process/water_level_process_new.h"
#include "ImageSensor.h"

class SingleImageInference : public ImageSensor
{
public:
    SingleImageInference(std::string file_path, int _queue_max_length, bool _is_full_drop);
    ~SingleImageInference();

    SegmentationInference *waterlevel_inference_rough = nullptr, *waterlevel_inference_fine = nullptr;

    void inference();
    void draw_waterlevel(cv::Mat& img, vector<double> line_equation);
    void save_image(string base_path);
    virtual cv::Mat getData() override;
    
    std::string m_file_path;
private:
    virtual void dataCollectionLoop() override;
    
}; 