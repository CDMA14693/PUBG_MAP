#include "pch.h"
#include "MainWindow.h"
#include "Setting.h"

std::vector<POINT> MainWindow::pointList;
RECT MainWindow::BackGround;
int MainWindow::POINT_100M=80;
int MainWindow::distanceBetweenPoints;
int MainWindow::distanceBetweenPoints_Game;
NOTIFYICONDATA MainWindow::Tray;
bool MainWindow::DrawPoint = false;
RECT MainWindow::WindowSize;

MainWindow& MainWindow::GteCreateWindow(HINSTANCE hinstance)
{
    static MainWindow my(hinstance);
    POINT p1{10,10}, p2{20,20};
    pointList.push_back(p1);
    pointList.push_back(p2);
    
    return my;
}



HWND MainWindow::GethWnd() const
{
    return hWnd;
}

//窗口构造与创建
MainWindow::MainWindow(HINSTANCE hinstance)
{

    //定义默认的窗口行为
    wc = {};
    wc.lpfnWndProc = this->WndProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = TEXT("My_window");
    //注册窗口
    RegisterClass(&wc);
    //保存全局Histance
    this->hinstance = hinstance;
    //创建窗口
    hWnd = CreateMyWindow();
    if (hWnd==nullptr)
    {
        exit(0);
    }
    //
    BackGround = { 10, 10, 400, 70 };
}

//窗口过程处理函数


HWND MainWindow::CreateMyWindow() const
{
    WindowSize = { 0,0 };
    HWND desktop = GetDesktopWindow();
    GetClientRect(desktop, &WindowSize);

    return CreateWindowEx(
        WS_EX_TRANSPARENT | WS_EX_LAYERED,  // 扩展窗口样式，使用 WS_EX_LAYERED 和 WS_EX_TRANSPARENT
        wc.lpszClassName,                   // 窗口类名
        TEXT("Transparent Window"),         // 窗口标题
        WS_POPUP,                           // 窗口样式
        0, 0,                               // 默认位置
        WindowSize.right, WindowSize.bottom,                         // 窗口宽度和高度
        nullptr,                            // 父窗口
        nullptr,                            // 菜单
        hinstance,                          // 应用程序实例句柄
        nullptr                             // 附加参数
    );

    
}

std::vector<POINT>& MainWindow::GetPoint()
{
    return pointList;
}

void MainWindow::updataDraw() 
{
    InvalidateRect(hWnd, NULL, true);
    UpdateWindow(hWnd);
}

void MainWindow::AddIconTray()
{
    ZeroMemory(&Tray, sizeof(Tray));

    Tray.cbSize = sizeof(NOTIFYICONDATA);
    Tray.hWnd = GethWnd();
    Tray.uID = 1; // 图标的 ID，可以自定义
    Tray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    Tray.uCallbackMessage = WM_USER + 1; // 自定义消息
    Tray.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); // IDI_TRAY_ICON 是你在资源中添加的图标 ID
    lstrcpy(Tray.szTip, _T("托盘图标提示文本"));

    Shell_NotifyIcon(NIM_ADD,&Tray);
}

void MainWindow::RemoveTray()
{
    Shell_NotifyIcon(NIM_DELETE, &Tray);
}

LRESULT MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_USER + 1:
    {
        switch (lParam)
        {
        case WM_RBUTTONDOWN:
            HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
            if (hMenu == NULL) {
                MessageBox(hwnd, TEXT("Failed to load menu resource"), TEXT("Error"), MB_OK);
                
                return 0;
            }

            HMENU hSubMenu = GetSubMenu(hMenu, 0);
            if (hSubMenu == NULL) {
                MessageBox(hwnd, TEXT("Failed to get submenu"), TEXT("Error"), MB_OK);
                DestroyMenu(hMenu);
                return 0;
            }


            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);

            TrackPopupMenu(hSubMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);

            DestroyMenu(hMenu);
            break;
        }
    }
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case ID_SETTING:
        {
            Setting s;
            s.DoModal();
        } 
            break;
        case ID_EXIT:
        {
            PostQuitMessage(0);
        }
            break;
        }
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // 创建一个兼容的内存DC
        HDC memDC = CreateCompatibleDC(hdc);
        //创建DC大小相同的位图
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
        //将位图选入DC
        SelectObject(memDC, memBitmap);

        RECT rect = { 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top };
        FillRect(memDC, &rect, (HBRUSH)GetStockObject(NULL_BRUSH)); //将透明内容完全填充至DC
        //绘图区间
        Draw(memDC);


        //绘图结束区间
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, memDC, 0, 0, SRCCOPY);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
       
    }
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

const void MainWindow::Draw(HDC & hdc)
{
    if (DrawPoint)
    {
        HBRUSH brushGreen = CreateSolidBrush(RGB(0, 255, 0));
        SelectObject(hdc, brushGreen);
        Ellipse(hdc, pointList[0].x - 5, pointList[0].y - 5, pointList[0].x + 5, pointList[0].y + 5);
        // 创建红色画刷并绘制第二个点
        HBRUSH brushRed = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(hdc, brushRed);
        Ellipse(hdc, pointList[1].x - 5, pointList[1].y - 5, pointList[1].x + 5, pointList[1].y + 5);
        // 删除画刷对象
        DeleteObject(brushGreen);
        DeleteObject(brushRed);
    }
    HBRUSH brushRed = CreateSolidBrush(RGB(255, 0, 255));
    FillRect(hdc,&BackGround , brushRed);

    std::wstring str1 = L"功能状态： ";
    if (DrawPoint)
    {
        str1 += L"开启";
    }
    else {
        str1 += L"关闭";
    }
    std::wstring str2 = L"地图100米像素数量： ";
    std::wstring str3 = L"2点之间像素数量： ";
    std::wstring str4 = L"游戏内距离： ";
    str2 += std::to_wstring(POINT_100M);
    str3 += std::to_wstring((int)(std::sqrt(std::pow(pointList[0].x - pointList[1].x, 2) + std::pow(pointList[0].y - pointList[1].y, 2))));
    str4 += std::to_wstring((int)(std::sqrt(std::pow(pointList[0].x - pointList[1].x, 2) + std::pow(pointList[0].y - pointList[1].y, 2)) * ((double)100 / POINT_100M)));
    TextOut(hdc, BackGround.left, BackGround.top, str1.c_str(), str1.length());
    TextOut(hdc, BackGround.left, BackGround.top+15, str2.c_str(), str2.length());
    TextOut(hdc, BackGround.left, BackGround.top+30, str3.c_str(), str3.length());
    TextOut(hdc, BackGround.left, BackGround.top+45, str4.c_str(), str4.length());
    
    DeleteObject(brushRed);
}