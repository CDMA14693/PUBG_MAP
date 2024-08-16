#pragma once
#include "Setting.h"



class MainWindow
{
public:
	static MainWindow& GteWindow();
	static void SetHinstance(HINSTANCE hinstanceF);
	HWND CreateWin() const;

	//防止使用复制构造函数。；
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
	void updataDraw();
	void SetKeyValue(std::map<int, std::vector<int>>);

	//关于 窗口绘制与计算的变量 ,目前赶进度先这样，
	static std::vector<POINT> pointList;
	static MainWindowInfo info;
	static int Map_Size;
	//添加托盘 
	void AddIconTray();
	//删除托盘
	void RemoveTray();
	static RECT WindowSize;
	static bool DrawPoint;
	static bool Map_open;
	


	//加载配置文件，
	bool LoadConfigurationBinary(const std::string& filename);
	static void SaveConfigurationBinary(const std::string& filename);
private:
	//函数
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//处理 自定义消息
	static void CALLBACK messageProc(int message,LPARAM lpramr);

	static std::vector<KeyboardRegister>::iterator it;

	HWND CreateMyWindow() const;
	MainWindow();
	const static void Draw(HDC &);

	//托盘 
	static NOTIFYICONDATA Tray;

	static HINSTANCE hinstance;
	//窗口全局变量
	HWND hWnd;
	
	WNDCLASS wc;
	
};

