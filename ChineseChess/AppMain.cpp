// 定义应用程序的入口点。
//
#include "framework.h"
#include "Resource.h"
#include "framework_base.h"
#include "thread_communicate.h"
#include "settings.h"

using namespace debugger;


bool g_inSizeMove = false;  // 是否处于调整状态
UINT_PTR g_renderTimer = 0; // 定时器ID
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
CHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
CHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

unsigned __stdcall RenderLoop(LPVOID lpParameter);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_CHINESECHESS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHINESECHESS));

    MSG msg;
    HANDLE hThreadRenderLoop;
    hThreadRenderLoop = (HANDLE)_beginthreadex(NULL, 0, RenderLoop, NULL, 0, NULL);	//创建线程
    if (hThreadRenderLoop == 0)
    {
        MessageBoxEx(NULL, "Failed to create RenderLoop thread", "Error", MB_OK | MB_ICONSTOP, NULL);
        quit_single = 1;
        return 255;
    }
    // 主消息循环:

    static float checkfocustime = 0;
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE /*| PM_QS_PAINT*/))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (quit_single)
        {
            break;
        }
        if (checkfocustime > 0.1)
        {
            FocusWindow = GetFocus();
            checkfocustime = 0;
        }
        else
        {
            checkfocustime += frmtm;
        }
    }
    DestroyWindow(hWnd);
    return 0;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHINESECHESS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_CHINESECHESS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindowA(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//  WM_DESTROY  - 发送退出消息并返回
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        normal_quit = 1;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        OnSize();
        break;
    case WM_MOUSEWHEEL:
        input_wheel(GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    case WM_ERASEBKGND:
        return 1; // 防止系统擦除背景造成闪烁
    default:
        
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}




void render()
{
    constexpr DXGI_PRESENT_PARAMETERS present_param = { 0,0,0,0 };
    HRESULT hr = S_OK;
    DrawCallNum = 0;
    debugger_main.reset();
    update();
    static TIMER timer;
    while (main_thread_render_blocked > 0)
    {
        if (main_thread_render_blocked == 1)
        {
            main_thread_render_blocked = 2;
            timer.start_timer();
        }

        Sleep(0);
        if (timer.stop_timer() > 16)
        {
            quit_single = 1;
        }
        return;
    }
    g_pD2DDeviceContext->BeginDraw();
    if (FAILED(hr))
    {
        debugger_main.writelog(-1, "Rending Error in D2DDeviceContext->BeginDraw(): Draw failed! " + to_string(hr));
        MessageBox(NULL, "Draw failed!", "Error", 0);
        return;
    }
    g_cm.update();
    rendPage();
    g_PageManager.update();
    g_PageManager.rend();
    g_cm.rend();
    rend_quit();

    if (page_index < 1002 && set2[0].show_framerate)
    {
        DrawTextA_1(to_string(fps) + " [" + to_string(DrawCallNum) + "]", g_pTextFormat, fpsRect, g_pBrushBlue);

    }
#ifdef _DEBUG

    debugger_main.add_output_line("cpos= " + to_string(cpos.x) + ", " + to_string(cpos.y));
#endif // _DEBUG
    debugger_main.rend();

    hr = g_pD2DDeviceContext->EndDraw();

    if (FAILED(hr))
    {
        debugger_main.writelog(-1, "Rending Error in D2DDeviceContext->EndDraw(): Draw failed! " + to_string(hr));
        MessageBox(NULL, "Draw failed!\n", "Rending Error", 0);
        Sleep(1000);
        quit_single = 1;
        return;
    }
    if (set2[0].vsync)
    {

        if (hWnd != FocusWindow && g_output)    //窗口失焦时降低帧率
        {
            g_output->WaitForVBlank();
            g_output->WaitForVBlank();
        }
        hr = g_pSwapChain->Present1(1, 0, &present_param);
    }
    else
    {
        hr = g_pSwapChain->Present1(0, 0, &present_param);
    }

    if (FAILED(hr))
    {
        debugger_main.writelog(DERROR, "Rending Error in g_pSwapChain->Present1(): " + to_string(hr));
        MessageBox(NULL, "Draw failed!\n", "Rending Error", 0);
        Sleep(1000);
        quit_single = 1;
        return;
    }
    return;
}


unsigned __stdcall RenderLoop(LPVOID lpParameter)
{
    init_once();
    
    if (quit_single == 1)   //初始化发生异常，退出
    {
        process_quit();
        DestroyWindow(hWnd);
        return -1;
    }
    page_index = 1000;


    while (1)
    {
        render();

        if (quit_single == 1)
        {
            process_quit();
            
            break;
        }
    }
    return 0;
}