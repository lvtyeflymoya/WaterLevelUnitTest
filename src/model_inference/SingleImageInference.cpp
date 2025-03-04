#include "SingleImageInference.h"


SingleImageInference::SingleImageInference()
{
    this->waterlevel_inference_rough = make_unique<SegmentationInference>("D:/Cpp_Project/PanoramicTracking/onnx_tensorRT/earlierModelFile/rough_waternet.engine", 512, 512, 3); // 水位线粗检测
    this->waterlevel_inference_fine = make_unique<SegmentationInference>("D:/Cpp_Project/PanoramicTracking/onnx_tensorRT/rough_waternet.engine", 2160, 256, 3);  // 水位线精检测
}

SingleImageInference::~SingleImageInference(){};

// SingleImageInference::~SingleImageInference()
// {
//     delete waterlevel_inference_rough;
//     delete waterlevel_inference_fine;
// }

void SingleImageInference::inference(const cv::Mat& input_image)
{
    // 保留原始图像的副本用于可视化
    this->original_image = input_image.clone();
    cv::Mat img_resize;
    cv::resize(this->original_image, img_resize, cv::Size(512, 512), cv::INTER_AREA);

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
        this->rough_result = waterlevel_inference_rough->do_inference(img_resize);
        int left_up_y = get_waterline_position(this->rough_result);
        int left_up_y_in_img = (int)(left_up_y / 521.0 * this->original_image.rows);
        rough_detected = (left_up_y != 0);
        // 能在图像中找到粗检测水位线邻域时，进一步进行精检测
        if (rough_detected)
        {
            // 精检测
            cv::Mat edge = this->original_image(cv::Rect(cv::Point2d(0, left_up_y_in_img), cv::Point2d(this->original_image.cols, min(left_up_y_in_img + 256, this->original_image.rows)))); // 根据原图像中的左上角y坐标裁剪邻域图像
            this->fine_result = waterlevel_inference_fine->do_inference(edge);    // 精检测模型推理

            vector<double> line_equation = fitting_waterline(this->fine_result, left_up_y_in_img, "inside");
            this->waterlevel_image = this->original_image.clone();
            draw_waterlevel(this->waterlevel_image, line_equation);
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


void SingleImageInference::save_image(const string& base_path)
{
    vector<pair<cv::Mat, string>> save_list = {
        {original_image, "_original.jpg"},
        {rough_result,   "_inference_rough_result.jpg"}
    };

    // 仅当粗检测成功时保存精检图和水位线图
    if (rough_detected) {
        save_list.emplace_back(fine_result,      "_inference_fine_result.jpg");
        save_list.emplace_back(waterlevel_image, "_waterline_display_result.jpg");
    }

    for (const auto& [img, suffix] : save_list) {
        if (!img.empty()) {
            string full_path = base_path + suffix;
            bool success = imwrite(full_path, img);
            cout << (success ? "已保存: " : "保存失败: ") << full_path << endl;
        }
    }
}