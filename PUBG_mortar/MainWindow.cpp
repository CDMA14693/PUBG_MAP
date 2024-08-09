#include "pch.h"
#include "MainWindow.h"
#include "HookHandler.h"
#include "Setting.h"



NOTIFYICONDATA MainWindow::Tray;
bool MainWindow::DrawPoint = false;
bool MainWindow::Map_open = false;
std::vector<KeyboardRegister>::iterator MainWindow::it;
RECT MainWindow::WindowSize;
std::vector<POINT> MainWindow::pointList;
MainWindowInfo MainWindow::info;
int MainWindow::Map_Size = 0;
HINSTANCE MainWindow::hinstance;

std::string GetExecutableDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}
std::string GetConfigFilePath() {
    std::string exeDir = GetExecutableDirectory();
    return exeDir + "\\config.dat"; // 使用相对路径
}


MainWindow& MainWindow::GteWindow()
{
    static MainWindow my;
    POINT p1{10,10}, p2{20,20};
    pointList.push_back(p1);
    pointList.push_back(p2);
    return my;
}

void MainWindow::SetHinstance(HINSTANCE hinstanceF)
{
    hinstance = hinstanceF;
}

HWND MainWindow::CreateWin() const
{
    return hWnd;
}

//窗口构造与创建
MainWindow::MainWindow()
{
    std::cout << "窗口chushihua" << std::endl;
    //定义默认的窗口行为
    wc = {};
    wc.lpfnWndProc = this->WndProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = TEXT("My_window");
    //注册窗口
    RegisterClass(&wc);
    //创建窗口
    hWnd = CreateMyWindow();
    if (hWnd==nullptr)
    {
        exit(0);
    }
    if (!LoadConfigurationBinary("config.dat"))
    {   
        std::cout<<"配置文件加载失败,使用默认配置"<< std::endl;
        info.BackGround = { 10, 10, 200, 100 };

        info.QuickKey[1] = { 0xA2 , 0x4D };
        info.QuickKey[2] = { 0x31,0x01 };
        info.QuickKey[3] = { 0x32,0x01 };
        info.QuickKey[4] = { 0x60 };
        info.QuickKey[5] = {};
        info.QuickKey[6] = {};
        info.QuickKey[7] = {};
        info.QuickKey[8] = { 0x4D };

        info.QuickKey[100] = { 0x1000 };
        info.QuickKey[101] = { 0x1001 };

        info.POINT_100M.push_back(100);
        info.POINT_100M.push_back(200);
        info.POINT_100M.push_back(300);
        info.POINT_100M.push_back(400);
        info.POINT_100M.push_back(500);

        info.PointSize = 5;
    }
    HookHandler &h= HookHandler::GetHookHandler();
    it= h.RegisterKeyboard(messageProc, info.QuickKey);
}
//创建窗口
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

void MainWindow::updataDraw() 
{
    InvalidateRect(hWnd, NULL, true);
    UpdateWindow(hWnd);
}

void MainWindow::SetKeyValue(std::map<int, std::vector<int>> Quickkey)
{
    info.QuickKey = Quickkey;
}

void MainWindow::AddIconTray()
{
    ZeroMemory(&Tray, sizeof(Tray));

    Tray.cbSize = sizeof(NOTIFYICONDATA);
    Tray.hWnd = CreateWin();
    Tray.uID = 1; // 图标的 ID，可以自定义
    Tray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    Tray.uCallbackMessage = WM_USER + 1; // 自定义消息
    Tray.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); // IDI_TRAY_ICON 是你在资源中添加的图标 ID
    lstrcpy(Tray.szTip, _T("地图计算器"));
    
    Shell_NotifyIcon(NIM_ADD,&Tray);
}

void MainWindow::RemoveTray()
{
    Shell_NotifyIcon(NIM_DELETE, &Tray);
}

bool MainWindow::LoadConfigurationBinary(const std::string& filename)
{
    std::ifstream file(GetConfigFilePath(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        MessageBox(NULL, L"Failed to open configuration file for reading:", L"", MB_OK);
        return false;
    }

    try {
        // Read POINT_100M vector
        uint32_t pointSize;
        file.read(reinterpret_cast<char*>(&pointSize), sizeof(pointSize));
        info.POINT_100M.resize(pointSize);
        file.read(reinterpret_cast<char*>(info.POINT_100M.data()), pointSize * sizeof(int));

        // Read BackGround
        file.read(reinterpret_cast<char*>(&info.BackGround), sizeof(info.BackGround));

        // Read PointSize
        file.read(reinterpret_cast<char*>(&info.PointSize), sizeof(info.PointSize));

        // Read QuickKey size
        uint32_t quickKeySize;
        file.read(reinterpret_cast<char*>(&quickKeySize), sizeof(quickKeySize));

        info.QuickKey.clear();
        for (uint32_t i = 0; i < quickKeySize; ++i) {
            // Read key
            int key;
            file.read(reinterpret_cast<char*>(&key), sizeof(key));

            // Read vector size
            uint32_t vecSize;
            file.read(reinterpret_cast<char*>(&vecSize), sizeof(vecSize));

            // Read vector elements
            std::vector<int> values(vecSize);
            if (vecSize > 0) {
                file.read(reinterpret_cast<char*>(values.data()), vecSize * sizeof(int));
            }

            info.QuickKey[key] = std::move(values);
        }

        file.close(); // Ensure the file is properly closed
    }
    catch (const std::exception& e) {
        MessageBox(NULL, L"Exception occurred while reading the binary file:", L"", MB_OK);
        return false;
    }

    return true;
}

void MainWindow::SaveConfigurationBinary(const std::string& filename)
{
    it->keyBindings = info.QuickKey;

    std::ofstream file(GetConfigFilePath(), std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        MessageBox(NULL, L"Failed to open configuration file for writing:", L"", MB_OK);
        return;
    }

    try {
        // Write POINT_100M vector
        uint32_t pointSize = static_cast<uint32_t>(info.POINT_100M.size());
        file.write(reinterpret_cast<const char*>(&pointSize), sizeof(pointSize));
        if (pointSize > 0) {
            file.write(reinterpret_cast<const char*>(info.POINT_100M.data()), pointSize * sizeof(int));
        }

        // Write BackGround
        file.write(reinterpret_cast<const char*>(&info.BackGround), sizeof(info.BackGround));

        // Write PointSize
        file.write(reinterpret_cast<const char*>(&info.PointSize), sizeof(info.PointSize));

        // Write QuickKey size
        uint32_t quickKeySize = static_cast<uint32_t>(info.QuickKey.size());
        file.write(reinterpret_cast<const char*>(&quickKeySize), sizeof(quickKeySize));

        for (const auto& el : info.QuickKey) {
            // Write key
            int key = el.first;
            file.write(reinterpret_cast<const char*>(&key), sizeof(key));

            // Write vector size
            uint32_t vecSize = static_cast<uint32_t>(el.second.size());
            file.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

            // Write vector elements
            if (vecSize > 0) {
                file.write(reinterpret_cast<const char*>(el.second.data()), vecSize * sizeof(int));
            }
        }

        file.flush();
        file.close(); // Ensure the file is properly closed
    }
    catch (const std::exception& e) {
        MessageBox(NULL, L"Exception occurred while writing the binary file:", L"", MB_OK);
    }
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
        switch (wmId) 
        {
        case ID_SETTING:
        {
            Setting s;
            s.Setinfo(info);
            s.DoModal();
        } 
            break;
        case ID_EXIT:
        {
            SaveConfigurationBinary("config.dat");
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
        
        // 绘制非透明区域，假设 info.BackGround 是 RECT 结构
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 选择你想要的颜色
        FillRect(memDC, &info.BackGround, hBrush);
        //绘图区间
        Draw(memDC);


        //绘图结束区间
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, memDC, 0, 0, SRCCOPY);
        DeleteObject(memBitmap);
        DeleteObject(hBrush);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
       
    }
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void MainWindow::messageProc(int message, LPARAM lpramr)
{
    std::cout << "消息" << message << "进入" << std::endl;
    MSLLHOOKSTRUCT* mouse = (MSLLHOOKSTRUCT*)lpramr;
    switch (message)
    {
    case 1:
    {
        DrawPoint = !DrawPoint;
        break;
    }
    
    case 2:
    {
        if (DrawPoint)
        {
            pointList.at(0) = mouse->pt;
        }
        
    }
        break;
    case 3:
    {
        if (DrawPoint)
        {
            pointList.at(1) = mouse->pt;
        }
    }
        break;
    case 4:
    {
        Map_open = !Map_open;
    }
    break;
    case 6:
    {
        if (Map_open)
        {
            pointList.at(0).x = WindowSize.right / 2;
            pointList.at(0).y = WindowSize.bottom / 2;
        }
       
    }
        break;
    case 7:
    {
        if (Map_open)
        {
            pointList.at(1).x = WindowSize.right / 2;
            pointList.at(2).y = WindowSize.bottom / 2;
        }
        
    }
        break;
    case 8: 
    {
        Map_open = !Map_open;
    }
        break;
    case 100:
    {
        if (Map_Size<info.POINT_100M.size()-1&& Map_open)
        {
            Map_Size++;
        }
        break;
    }
    case 101:
    {
        if (Map_Size > 0&& Map_open)
        {
            Map_Size--;
        }
        break;

    }
    }
    MainWindow& m=MainWindow::GteWindow();
    m.updataDraw();
}

const void MainWindow::Draw(HDC & hdc)
{
    if (DrawPoint)
    {
        HBRUSH brushGreen = CreateSolidBrush(RGB(0, 255, 0));
        SelectObject(hdc, brushGreen);
        Ellipse(hdc, pointList[0].x - info.PointSize, pointList[0].y - info.PointSize, pointList[0].x + info.PointSize, pointList[0].y + info.PointSize);
        // 创建红色画刷并绘制第二个点
        HBRUSH brushRed = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(hdc, brushRed);
        Ellipse(hdc,pointList[1].x - info.PointSize, pointList[1].y - info.PointSize, pointList[1].x + info.PointSize, pointList[1].y + info.PointSize);
        // 删除画刷对象
        DeleteObject(brushGreen);
        DeleteObject(brushRed);
    }
    HBRUSH brushRed = CreateSolidBrush(RGB(255, 0, 255));
    FillRect(hdc,&info.BackGround , brushRed);

    std::wstring str1 = L"功能状态： ";
    if (DrawPoint)
    {
        str1 += L"开启";
    }
    else {
        str1 += L"关闭";
    }
    
    std::wstring str2 = L"软件认为地图状态: ";
    if (Map_open)
    {
        str2 += L"开启";
    }
    else {
        str2 += L"关闭";
    }
    std::wstring str3 = L"软件认为地图缩放次数: ";
    std::wstring str4 = L"地图100米像素数量： ";
    std::wstring str5 = L"2点之间像素数量： ";
    std::wstring str6 = L"游戏内距离： ";
    str3 += std::to_wstring(Map_Size+1);
    str4 += std::to_wstring(info.POINT_100M[Map_Size]);
    str5 += std::to_wstring((int)(std::sqrt(std::pow(pointList[0].x - pointList[1].x, 2) + std::pow(pointList[0].y - pointList[1].y, 2))));
    str6 += std::to_wstring((int)(std::sqrt(std::pow(pointList[0].x - pointList[1].x, 2) + std::pow(pointList[0].y - pointList[1].y, 2)) * ((double)100 / info.POINT_100M[0])));
    TextOut(hdc, info.BackGround.left, info.BackGround.top, str1.c_str(), str1.length());
    TextOut(hdc, info.BackGround.left, info.BackGround.top+15, str2.c_str(), str2.length());

    TextOut(hdc, info.BackGround.left, info.BackGround.top + 30, str3.c_str(), str3.length());
    TextOut(hdc, info.BackGround.left, info.BackGround.top + 45, str4.c_str(), str4.length());
    TextOut(hdc, info.BackGround.left, info.BackGround.top + 60, str5.c_str(), str5.length());
    TextOut(hdc, info.BackGround.left, info.BackGround.top+75, str6.c_str(), str6.length());
    
    DeleteObject(brushRed);
}
