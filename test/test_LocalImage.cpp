#include "LocalImage.h"

int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    static plog::ColorConsoleAppender<plog::MyFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    PLOGI << "System initialized";

    LocalImage localImage("F:/MasterGraduate/03-Code/PanoramicTracking/datasets/images/data4",
                          10, false);
    localImage.start();

    while (1)
    {
        cv::imshow("LocalImage", localImage.getData());
        cv::waitKey(100);
    }
}