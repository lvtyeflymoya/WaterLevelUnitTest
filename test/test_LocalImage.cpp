#include "LocalImage.h"

int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    static plog::ColorConsoleAppender<plog::MyFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    PLOGI << "System initialized";

    // LocalImage localImage("D:/ImageAnnotation/chuanzha/Fabricate/306.jpg",
    //                       10, false);
    LocalImage localImage("D:/ImageAnnotation/chuanzha/Fabricate",
        10, false);
    localImage.start();

    while (1)
    {
        cv::imshow("LocalImage", localImage.getData());
        cv::waitKey(100);

    }
}