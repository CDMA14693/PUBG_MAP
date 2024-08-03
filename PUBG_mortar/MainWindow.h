#pragma once
#include "Setting.h"

class MainWindow
{
public:
	static MainWindow& GteCreateWindow(HINSTANCE hinstance);

	HWND GethWnd() const;

	//防止使用复制构造函数。；
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
	static std::vector<POINT>& GetPoint();
	void updataDraw();

	//关于 窗口绘制与计算的变量 ,目前赶进度先这样，
	static int POINT_100M;
	static int distanceBetweenPoints;
	static int distanceBetweenPoints_Game;

	void AddIconTray();
	void RemoveTray();

	static RECT WindowSize;
	static bool DrawPoint;
private:
	//函数
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND CreateMyWindow() const;
	MainWindow(HINSTANCE hinstance);
	const static void Draw(HDC &);

	static std::vector<POINT> pointList;
	static RECT BackGround;

	

	//托盘 
	static NOTIFYICONDATA Tray;

	HINSTANCE hinstance;
	//窗口全局变量
	HWND hWnd;
	
	WNDCLASS wc;
	
};

