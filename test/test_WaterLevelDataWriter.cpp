#include "WaterLevelPrediction.h"
#include "LocalImage.h"
#include <filesystem>
#include <unordered_set>

namespace fs = std::filesystem;

int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    static plog::ColorConsoleAppender<plog::MyFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    const string input_folder = "D:/ImageAnnotation/chuanzha/Fabricate";
    const std::string output_json_path = "D:/Cpp_Project/PanoramicTracking/results/waterlevel.json";
    
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

    LocalImage image_reader(input_folder, 80, false);
    image_reader.start();
    std::vector<WaterLeveResult> results;
    WaterLeveResult result;
    WaterLevelDateWriter water_level_predictor;
    // 持续读取直到读完文件夹中的所有图片
    int idx = 0;
    while(image_reader.isRunning()) 
    {
        cv::Mat src_img = image_reader.getData();
        
        string base_name = std::to_string(idx++);
        result = water_level_predictor.processSingleImage(src_img, base_name);
        results.push_back(result);
        PLOGV << "正在处理" << idx << "张图片";

        if(idx > (imageCount - 1)){
            image_reader.stop();
            break;
        }
    }
    water_level_predictor.saveResults(results, output_json_path);

    cout << "全部图片处理完成！" << endl;
    return 0;
}
