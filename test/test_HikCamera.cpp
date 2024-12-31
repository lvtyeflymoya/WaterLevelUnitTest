#include "HikCamera.h"

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

HWND CreatePreviewWindow(HINSTANCE hInstance)
{
    // 定义窗口类
    const char CLASS_NAME[] = "PreviewWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;   // 窗口过程函数
    wc.hInstance = hInstance;      // 当前实例句柄
    wc.lpszClassName = CLASS_NAME; // 窗口类名称

    // 注册窗口类
    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd = CreateWindowEx(
        0,                            // 扩展样式
        CLASS_NAME,                   // 窗口类名称
        "Preview Window",             // 窗口标题
        WS_OVERLAPPEDWINDOW,          // 窗口样式
        CW_USEDEFAULT, CW_USEDEFAULT, // 初始位置
        384, 216,                     // 窗口宽高
        NULL,                         // 父窗口句柄
        NULL,                         // 菜单句柄
        hInstance,                    // 实例句柄
        NULL                          // 额外参数
    );

    if (hwnd == NULL)
    {
        printf("Failed to create window\n");
        return NULL;
    }

    // 显示窗口
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;
}


int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    static plog::ColorConsoleAppender<plog::MyFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    PLOGI << "System initialized";


    // 预览窗口创建
    HINSTANCE hInstance = GetModuleHandle(NULL); // 获取当前模块句柄
    HWND hWnd = CreatePreviewWindow(hInstance);  // 创建新窗口
    if (hWnd == NULL)
    {
        printf("Failed to create preview window\n");
        return -1;
    }

    HikCamera camera(10, true, hWnd);
    camera.start();

    while (1)
    {
        cv::imshow("LocalImage", camera.getData());
        cv::waitKey(100);
    }
}