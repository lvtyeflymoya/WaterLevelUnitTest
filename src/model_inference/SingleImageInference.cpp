#include "SingleImageInference.h"


SingleImageInference::SingleImageInference()
{
    this->waterlevel_inference_rough = make_unique<SegmentationInference>("D:/Cpp_Project/PanoramicTracking/onnx_tensorRT/earlierModelFile/rough_waternet.engine", 512, 512, 3); // 水位线粗检测
    this->waterlevel_inference_fine = make_unique<SegmentationInference>("D:/Cpp_Project/PanoramicTracking/onnx_tensorRT/rough_waternet.engine", 2160, 256, 3);  // 水位线精检测
    this->waterLine_equation = vector<double>(3, 0);
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
            this->waterLine_equation = line_equation;
            this->waterlevel_image = this->original_image.clone();
            draw_waterlevel(this->waterlevel_image, line_equation);
        }
    }
}


// 在原图上绘制水位线
void SingleImageInference::draw_waterlevel(cv::Mat& img, std::vector<double> line_equation)
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


void SingleImageInference::save_image(const std::string& base_path)
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

// 计算水位线位置
double SingleImageInference::calculateWaterLine()
{
    double A = this->waterLine_equation[0], B = this->waterLine_equation[1], C = this->waterLine_equation[2];
    //  当a,b,c均为0时，未检测到水位线，返回-1
    if (A == 0 && B == 0 && C == 0)
    {
        return -1;
    }
    // else
    // {
    //     return -(A * 100 + C) / B;
    // }

    std::vector<std::pair<double, double>> inside_pairs = {
        {8, 181},
        {7, 219},
        {6, 211},
        {5, 208},
        {4, 204},
        {3, 197},
        {2, 192},
        {1, 191},
        {0.3, 192}
    };
    std::vector<std::pair<double, double>> outside_pairs = {
        {4, 228},
        {3, 321},
        {2, 312},
        {1, 297},
        {0, 297}
    };
    double inside_calibration_location_x = 1000;
    double outside_calibration_location_x = 1350;
    double calibration_location_x = inside_calibration_location_x;
    std::vector<std::pair<double, double>>pixel_world_mapping_relation = inside_pairs;
    //* 代入水位标定点的x坐标，计算水位的 `像素坐标`
    double waterline_y = -(A * calibration_location_x + C) / B;

    //* 采用分段线性插值的方法计算水位的 `世界坐标`
    if (waterline_y >= pixel_world_mapping_relation[0].second) // !水位的像素坐标在最上面的标定点的下方
    {
        for (int i = 0; i < pixel_world_mapping_relation.size(); i++)
        {
            pair<double, double> _pair = pixel_world_mapping_relation[i];
            if (waterline_y >= _pair.second)
            {
                waterline_y -= _pair.second;
            }
            else
            {
                double unit = pixel_world_mapping_relation[i - 1].first - pixel_world_mapping_relation[i].first; // 相邻两个标定点间的实际距离
                double calculated_water_level = _pair.first + (_pair.second - waterline_y) / _pair.second * unit;              // 线性插值
                return calculated_water_level;
            }
        }
    }
    else{
        // !水位的像素坐标在最上面的标定点的上方
        return -1;
    }


}

