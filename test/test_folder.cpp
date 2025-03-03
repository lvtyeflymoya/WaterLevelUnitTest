// 读取文件夹所有图片进行推理和水位线计算，并保存可视化结果
#include "SingleImageInference.h"
#include "LocalImage.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
namespace fs = std::filesystem;

static bool ends_with(const std::string &str, const std::string &suffix)
{
    if (str.size() < suffix.size())
        return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

int main()
{
    // auto localImage("D:/ImageAnnotation/chuanzha/Fabricate",10, false);
    // localImage.start(); // 将图像读取到队列中

    std::string folderPath = "D:/ImageAnnotation/chuanzha/Fabricate"; 
    std::string outputFolder = folderPath + "/results"; // 输出目录
    if (!fs::exists(outputFolder)) {
        fs::create_directories(outputFolder);
    }
    // 遍历文件夹
    for (const auto &entry : fs::directory_iterator(folderPath))
    {
        if (fs::is_regular_file(entry))
        {
            std::string file_path = entry.path().string();
            std::string file_name = entry.path().stem().string();
            if (ends_with(file_path, ".jpg") || ends_with(file_path, ".png"))
            {
                SingleImageInference signal_image_inference(file_path, 50, false);
                signal_image_inference.inference();
                size_t lastdot = file_path.find_last_of(".");
                std::string output_basepath = outputFolder + '/' + file_name;
                signal_image_inference.save_image(output_basepath);
            }
        }
    }

}
