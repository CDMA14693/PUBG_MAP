#include"pch.h"
#include"MainWindow.h"
#include"HookHandler.h"


struct Myinfo {
    std::map<std::string, std::vector<int>> key;
    int POINT_100M=80;
};



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
    {
        wprintf(L"错误: MFC 初始化失败\n");
        return 1;
    }


    /*AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    std::cout << "Hello, console!\n " << std::endl;*/


    
    MainWindow& m = MainWindow::GteWindow();
    m.SetHinstance(hInstance);
    m.CreateWin();
    HWND hwnd = m.CreateWin();
    std::cout << "添加托盘" << std::endl;
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

    HookHandler& h = HookHandler::GetHookHandler();
    h.installHook();
    
    m.AddIconTray();
    std::cout << "添加托盘" << std::endl;
    MSG msg = {};
    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)
            break;
    }
    m.RemoveTray();

    h.UnistallHook();
    return 0;
}
