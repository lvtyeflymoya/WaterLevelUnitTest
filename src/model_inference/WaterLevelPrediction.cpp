#include "WaterLevelPrediction.h"
#include <cereal/types/vector.hpp>


WaterLevelDateWriter::WaterLevelDateWriter(){};
WaterLevelDateWriter::~WaterLevelDateWriter(){};

WaterLeveResult WaterLevelDateWriter::processSingleImage(cv::Mat& img, std::string& img_name) {
    WaterLeveResult result;
   
    SingleImageInference inference;
    inference.inference(img);
    
    result.filename = img_name;
    result.waterline_height = inference.calculateWaterLine();
    result.laser_value = generateRandomValue(result.waterline_height);
    result.pressure_value = generateRandomValue(result.waterline_height);
    
    return result;
}

// 保存结果到json
void WaterLevelDateWriter::saveResults(const std::vector<WaterLeveResult>& results, const std::string& json_path) {
    std::ofstream ofs(json_path);
    {
        cereal::JSONOutputArchive archive(ofs);
        archive(cereal::make_nvp("results", results));
    }
    std::cout << "已保存结果至: " << json_path << std::endl;
}

// 根据水位线高度生成随机值
double WaterLevelDateWriter::generateRandomValue(double waterline_height) {
    std::random_device rd;
    std::mt19937 gen(rd());
    if(waterline_height != 0.0 &  waterline_height != -1.0){
        // 假设范围是水位线高度的正负10%
        std::uniform_real_distribution<> dis(waterline_height * 0.9, waterline_height * 1.1);
        return dis(gen);
    }
    else{
        return 0.0;
    }
    
}


WaterLevelDataReader::WaterLevelDataReader(const std::string& _json_path):json_path(_json_path){};

// 读取json文件
std::vector<WaterLeveResult> WaterLevelDataReader::readData() {
    std::vector<WaterLeveResult> results;
        std::ifstream ifs(json_path);
        if (ifs.is_open()) {
            cereal::JSONInputArchive archive(ifs);
            archive(cereal::make_nvp("results", results));
        }
        return results;
}