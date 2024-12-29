#include "plog/Log.h"
#include "plog/Init.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include "plog/Formatters/TxtFormatter.h"
int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);
    PLOGI << "System initialized";

    return 0;
}