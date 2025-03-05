#pragma once
#include "SingleImageInference.h"
#include <cereal/archives/json.hpp>
#include <filesystem>
#include <fstream>
#include <optional>
#include <opencv2/opencv.hpp>


struct WaterLeveResult {
    std::string filename;
    double waterline_height;
    double laser_value;
    double pressure_value;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(
            CEREAL_NVP(filename),
            CEREAL_NVP(waterline_height),
            CEREAL_NVP(laser_value),
            CEREAL_NVP(pressure_value)
        );
    }
};

class WaterLevelDateWriter {
public:
    WaterLevelDateWriter();
    ~WaterLevelDateWriter();
    WaterLeveResult processSingleImage(cv::Mat& img, std::string& img_name);
    void saveResults(const std::vector<WaterLeveResult>& results, const std::string& path);
    double generateRandomValue(double waterline_height);    // 根据水位线高度生成随机值
};

class WaterLevelDataReader{
public:
    WaterLevelDataReader(const std::string& _json_path);
    std::vector<WaterLeveResult> readData();
private:
    std::string json_path;
};
