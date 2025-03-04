#include "SingleImageInference.h"
#include "LocalImage.h"
#include <filesystem>
#include <unordered_set>

namespace fs = std::filesystem;

int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    const string input_folder = "D:/ImageAnnotation/chuanzha/Fabricate";
    const string output_folder = "D:/ImageAnnotation/chuanzha/Fabricate/results";
    
    // 统计输入目录下的图片数量
    int imageCount = 0;
    const std::unordered_set<std::string> imageExts{".jpg", ".jpeg", ".png"};
    for (const auto& entry : fs::directory_iterator(input_folder)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            
            if (imageExts.count(ext)) {
                ++imageCount;
            }
        }
    }

    // 创建输出目录
    if (!fs::exists(output_folder)) {
        fs::create_directories(output_folder);
    }

    LocalImage image_reader(input_folder, 50, false);
    image_reader.start();

    // 持续读取直到完成
    int idx = 0;
    while(image_reader.isRunning()) 
    {
        cv::Mat src_img = image_reader.getData();

        SingleImageInference processor;
        processor.inference(src_img);

        // 构造包含原文件名的保存路径
        string base_name = std::to_string(idx++) + ".jpg";
        string save_path = output_folder + "/" + base_name;

        // 保存四张结果图
        processor.save_image(save_path);
        if(idx > (imageCount - 1)){
            break;
        }
    }

    cout << "全部图片处理完成！" << endl;
    return 0;
}
