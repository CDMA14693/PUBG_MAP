#include <windows.h>
#include <iostream>
#include <stdio.h>  // For freopen
#include <string>
#include <dwmapi.h>
#include <math.h>

#pragma comment(lib, "dwmapi.lib")

HHOOK hMouseHook = nullptr, keyboardHook = nullptr;
bool g_HandleClick = false;
HWND hwnd;
POINT p1 = { 0, 0 }, p2 = {0,0};
RECT bgc = { 10, 10, 400, 70 };
int POINT_100M = 80;
int distance_M = 0;
int POINT_SIZE=0;
RECT desktopRect;

void update_POINT_100M() {
    POINT_SIZE = (int)(std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2))) ;
    distance_M = (int)(std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2)) * ((double)100/POINT_100M ));
    std::cout << "像素数量" << std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2)) << std::endl;
    
};


// 鼠标钩子过程函数
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        if (wParam == WM_LBUTTONDOWN||wParam==WM_RBUTTONDOWN) {
            MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;
            if (pMouseStruct != nullptr) {
                POINT pt = pMouseStruct->pt;
                ScreenToClient(hwnd, &pt);
                if (g_HandleClick) {
                    if (wParam == WM_RBUTTONDOWN)
                    {
                        p2 = pt;
                    }
                    else
                    {
                        p1 = pt;
                    }
                    update_POINT_100M();
                    InvalidateRect(hwnd, NULL, true);
                    UpdateWindow(hwnd);
                    // 阻止消息传递给下一个钩子
                }
            }
        }
    }
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

//键盘钩子过程函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        
        

        if (wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;

            bool ctrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x8000;
            bool plusPressed = (kbStruct->vkCode == VK_OEM_PLUS || kbStruct->vkCode == VK_ADD);
            bool minusPressed = (kbStruct->vkCode == VK_OEM_MINUS || kbStruct->vkCode == VK_SUBTRACT);
            if (kbStruct->vkCode == VK_F6) {
                std::cout << "F6 key pressed!" << std::endl;
                g_HandleClick = !g_HandleClick;
                InvalidateRect(hwnd, NULL, true);
                UpdateWindow(hwnd);
            }
            if (ctrlPressed && plusPressed) {
                std::cout << "Ctrl + Pressed" << std::endl;
                POINT_100M += 5;
                std::cout << "POINT_100M: " << POINT_100M << std::endl;
                InvalidateRect(hwnd, NULL, true);
                UpdateWindow(hwnd);
            } 

            if (ctrlPressed && minusPressed) {
                std::cout << "Ctrl - Pressed" << std::endl;
                POINT_100M -= 5;
                std::cout << "POINT_100M: " << POINT_100M << std::endl;
                InvalidateRect(hwnd, NULL, true);
                UpdateWindow(hwnd);
            }
            if (kbStruct->vkCode == VK_SPACE) {
                p1.x = desktopRect.right / 2;
                p1.y = desktopRect.bottom / 2;
                std::cout << "spaceback"<<std::endl;
                update_POINT_100M();
                InvalidateRect(hwnd, NULL, true);
                UpdateWindow(hwnd);  
            }
        }
    }
    // 调用下一个钩子
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}
// 安装钩子
void InstallMouseHook() {
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, nullptr, 0);
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hMouseHook == nullptr) {
        MessageBox(nullptr, L"Failed to install mouse hook!", L"Error", MB_ICONERROR);
    }
    if (keyboardHook == nullptr) {
        MessageBox(nullptr, L"Failed to install keyboar hook!", L"Error", MB_ICONERROR);
    }
}

// 卸载钩子
void UninstallMouseHook() {
    if (hMouseHook != nullptr) {
        UnhookWindowsHookEx(hMouseHook);
    }
    if (keyboardHook == nullptr) {
        UnhookWindowsHookEx(keyboardHook);
    }
}

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    switch (msg) {
    case WM_CREATE: {
        InstallMouseHook();
        break;
    }
    case WM_PAINT: {

        update_POINT_100M();
        PAINTSTRUCT ps;
        
        HDC hdc = BeginPaint(hwnd, &ps);

        // 创建一个兼容的内存DC
        HDC memDC = CreateCompatibleDC(hdc);
        //创建DC大小相同的位图
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
        //将位图选入DC
        SelectObject(memDC, memBitmap);

        // 用透明背景清除内存DC
        BLENDFUNCTION blend = { 0 };
        blend.BlendOp = AC_SRC_OVER;
        blend.BlendFlags = 0;
        blend.SourceConstantAlpha = 0; // 完全透明
        blend.AlphaFormat = 0;

        RECT rect = { 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top };
        FillRect(memDC, &rect, (HBRUSH)GetStockObject(NULL_BRUSH)); //将透明内容完全填充至DC

       
        // 创建绿色画刷并绘制第一个点
        HBRUSH brushGreen = CreateSolidBrush(RGB(0, 255, 0));
        SelectObject(memDC, brushGreen);
        if (g_HandleClick)
        {
            Ellipse(memDC, p1.x - 5, p1.y - 5, p1.x + 5, p1.y + 5);
        }

        // 创建红色画刷并绘制第二个点
        HBRUSH brushRed = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(memDC, brushRed);
        
        if (g_HandleClick)
        {
            Ellipse(memDC, p2.x - 5, p2.y - 5, p2.x + 5, p2.y + 5);
        }
        // 删除画刷对象
        DeleteObject(brushGreen);
        DeleteObject(brushRed);
        
        brushRed= CreateSolidBrush(RGB(255, 0, 255));

        FillRect(memDC, &bgc, brushRed);

        std::wstring str1,str2,str3;
        str3 += L" 当前2点之间像素数量: ";
        str3 += std::to_wstring(POINT_SIZE);
        str3 += L"像素";
        str1 += L"一百米像素数量： ";
        str1+=std::to_wstring(POINT_100M);
        str1 += L" 像素";
        str2 += L"游戏内距离： ";
        str2 += std::to_wstring(distance_M);
        std::cout << "距离: " << distance_M << std::endl;
        str2 += L" 米";

        TextOut(memDC, 10, 10, str3.c_str(), str3.length());
        TextOut(memDC, 10, 30, str1.c_str(), str1.length());
        TextOut(memDC, 10, 50, str2.c_str(), str2.length());

        
        
        
        
        
        
        // 将内存DC的内容拷贝到窗口DC
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, memDC, 0, 0, SRCCOPY);

        

        // 清理资源
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
        break;
    }
        
    case WM_DESTROY:
        UninstallMouseHook();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //适应性DPI 点对点渲染
    SetProcessDPIAware();


    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    std::cout << "Hello, console!\n " << std::endl;


    
    HWND desktop = GetDesktopWindow();
    GetClientRect(desktop, &desktopRect);

    // 定义窗口类
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("TransparentWindowClass");

    // 注册窗口类
    RegisterClass(&wc);

    // 创建窗口
    hwnd = CreateWindowEx(
        WS_EX_TRANSPARENT | WS_EX_LAYERED,  // 扩展窗口样式，使用 WS_EX_LAYERED 和 WS_EX_TRANSPARENT
        wc.lpszClassName,                   // 窗口类名
        TEXT("Transparent Window"),         // 窗口标题
        WS_POPUP,                           // 窗口样式
        0, 0,                               // 默认位置
        desktopRect.right, desktopRect.bottom,                         // 窗口宽度和高度
        nullptr,                            // 父窗口
        nullptr,                            // 菜单
        hInstance,                          // 应用程序实例句柄
        nullptr                             // 附加参数
    );

    if (hwnd == nullptr) {
        return 0;
    }

    // 使窗口透明
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

    // 设置 DWM 透明区域属性
    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);


    // 显示窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // 消息循环
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
