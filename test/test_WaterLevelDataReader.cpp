#include "WaterLevelPrediction.h"

int main() {
    WaterLevelDataReader reader("D:/Cpp_Project/PanoramicTracking/results/waterlevel.json");
    std::vector<WaterLeveResult> results = reader.readData();
    for (const auto& result : results){
        std::cout << "图片名: " << result.filename << std::endl;
        std::cout << "水位线高度: " << result.waterline_height << std::endl;
        std::cout << "激光值: " << result.laser_value << std::endl;
        std::cout << "压力值: " << result.pressure_value << std::endl;
        std::cout << std::endl;
    }
}