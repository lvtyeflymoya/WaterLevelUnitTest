// 测试单张图片推理
#include "SingleImageInference.h"


int main()
{
    SingleImageInference signal_image_inference("D:/ImageAnnotation/chuanzha/Fabricate/312.jpg", 5,false);
    signal_image_inference.inference();
    signal_image_inference.save_image("C:/Users/Zhang/Desktop/111/");
}