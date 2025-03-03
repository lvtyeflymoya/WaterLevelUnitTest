#include "SingleImageInference.h"


SingleImageInference::SingleImageInference(std::string file_path, int _queue_max_length, bool _is_full_drop)
: ImageSensor(_queue_max_length, 500, _is_full_drop)
{
    this->waterlevel_inference_rough = new SegmentationInference("D:/Cpp_Project/PanoramicTracking/onnxANDtensorRT/earlierModelFile/rough_waternet.engine", 512, 512, 3); // 水位线粗检测
    this->waterlevel_inference_fine = new SegmentationInference("D:/Cpp_Project/PanoramicTracking/onnxANDtensorRT/rough_waternet.engine", 2160, 256, 3);  // 水位线精检测
    this->m_file_path = file_path;
}


SingleImageInference::~SingleImageInference()
{
    delete waterlevel_inference_rough;
    delete waterlevel_inference_fine;
}

void SingleImageInference::inference()
{
    cv::Mat img = cv::imread(this->m_file_path);
    this->enqueueData(img);
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
        cv::Mat rough_output = waterlevel_inference_rough->do_inference(img_resize);
        this->enqueueData(rough_output);
        int left_up_y = get_waterline_position(rough_output);
        int left_up_y_in_img = (int)(left_up_y / 521.0 * img.rows);
        // 能在图像中找到粗检测水位线邻域时，进一步进行精检测
        if (left_up_y != 0)
        {
            // 精检测
            cv::Mat edge = img(cv::Rect(cv::Point2d(0, left_up_y_in_img), cv::Point2d(img.cols, min(left_up_y_in_img + 256, img.rows)))); // 根据原图像中的左上角y坐标裁剪邻域图像
            cv::Mat fine_output = waterlevel_inference_fine->do_inference(edge);    // 精检测模型推理
            this->enqueueData(fine_output);

            vector<double> line_equation = fitting_waterline(fine_output, left_up_y_in_img, "inside");
            cv::Mat waterline_output = img;
            draw_waterlevel(waterline_output, line_equation);
            this->enqueueData(waterline_output);
        }
    }
}


// 在原图上绘制水位线
void SingleImageInference::draw_waterlevel(cv::Mat& img, vector<double> line_equation)
{
    double A = line_equation[0], B = line_equation[1], C = line_equation[2];
    cv::Point2d ptStart, ptEnd; // 水位线的起始点和终止点
    ptStart.x = 0;
    ptStart.y = -(A * ptStart.x + C) / B;
    ptEnd.x = img.cols - 1;
    ptEnd.y = -(A * ptEnd.x + C) / B;
    int line_width = 3;                                                  // 判断当前显示结果的框是否被放大，使用不同粗细的线绘制
    cv::line(img, ptStart, ptEnd, cv::Scalar(0, 0, 255), line_width, 8); // 画线
}

void SingleImageInference::dataCollectionLoop(){}

cv::Mat SingleImageInference::getData()
{
    std::unique_lock<std::mutex> lock(mutex);
    if (this->images.empty())
    {
        PLOGV << "Blocking wait for new data...";
        cv.wait(lock);
    }
    cv::Mat img = this->images.front();
    this->images.pop_front();
    return img;
}

void SingleImageInference::save_image(string base_path)
{
    vector<string> filenames = 
    {
        "_original.jpg",
        "_inference_rough_result.jpg",
        "_inference_fine_result.jpg",
        "_waterline_display_result.jpg"
    };
    int num = this-> images.size();
    for (size_t i = 0; i < num; i++) 
    {
        string full_path = base_path + filenames[i];
        bool success = imwrite(full_path, this->images.front());
        this->images.pop_front();
    
        cout << "已保存: " << full_path << endl;
    }
}