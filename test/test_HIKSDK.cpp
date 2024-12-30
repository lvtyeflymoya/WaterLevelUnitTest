#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "HCNetSDK.h"
#include <time.h>
using namespace std;

typedef HWND(WINAPI *PROCGETCONSOLEWINDOW)();
PROCGETCONSOLEWINDOW GetConsoleWindowAPI;

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    switch (dwType)
    {
    case EXCEPTION_RECONNECT: // 预览时重连
        printf("----------reconnect--------%d\n", time(NULL));
        break;
    default:
        break;
    }
}

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
        800, 600,                     // 窗口宽高
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

void main()
{

    //---------------------------------------
    // 初始化
    NET_DVR_Init();
    // 设置连接时间与重连时间
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);

    //---------------------------------------
    // 设置异常消息回调函数
    NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

    //---------------------------------------
    // 获取控制台窗口句柄
    HMODULE hKernel32 = GetModuleHandle("kernel32");
    GetConsoleWindowAPI = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow");

    //---------------------------------------
    // 注册设备
    LONG lUserID;

    // 登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    struLoginInfo.bUseAsynLogin = 0;                      // 同步登录方式
    strcpy(struLoginInfo.sDeviceAddress, "192.168.1.64"); // 设备IP地址
    struLoginInfo.wPort = 8000;                           // 设备服务端口
    strcpy(struLoginInfo.sUserName, "admin");             // 设备登录用户名
    strcpy(struLoginInfo.sPassword, "waterline123456");   // 设备登录密码

    // 设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if (lUserID < 0)
    {
        printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return;
    }

    //---------------------------------------
    // 启动预览并设置回调数据流
    LONG lRealPlayHandle;
    HINSTANCE hInstance = GetModuleHandle(NULL); // 获取当前模块句柄
    HWND hWnd = CreatePreviewWindow(hInstance);  // 创建新窗口
    if (hWnd == NULL)
    {
        printf("Failed to create preview window\n");
        return;
    }
    NET_DVR_PREVIEWINFO struPlayInfo = {0};
    struPlayInfo.hPlayWnd = hWnd;  // 需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel = 1;     // 预览通道号
    struPlayInfo.dwStreamType = 0; // 0-主码流，1-子码流，2-码流3，3-码流4，以此类推
    struPlayInfo.dwLinkMode = 0;   // 0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked = 1;     // 0- 非阻塞取流，1- 阻塞取流

    lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);
    if (lRealPlayHandle < 0)
    {
        printf("NET_DVR_RealPlay_V40 error\n");
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
    }

    // 消息循环
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    //---------------------------------------
    // 关闭预览
    NET_DVR_StopRealPlay(lRealPlayHandle);
    // 注销用户
    NET_DVR_Logout(lUserID);
    // 释放SDK资源
    NET_DVR_Cleanup();

    return;
}
