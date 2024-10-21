#pragma once
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>  // For freopen
#include <string>
#include <dwmapi.h>
#include <math.h>
#include <vector>
#include <map>
#include <unordered_set>
#include <fstream>
#include <functional>


//
#include"resource.h"


//lib 加载
#pragma comment(lib, "dwmapi.lib")

#include <afxwin.h>  // MFC 核心和标准组件
#include <afxext.h> 
#include <afxdialogex.h>


struct MainWindowInfo
{
	std::vector<int> POINT_100M;
	std::map<int, std::vector<int>> QuickKey;
	RECT BackGround;
	int PointSize;
	BOOL console;
	MainWindowInfo() : PointSize(10) {
		BackGround = { 0,0,100,100 }; console = false;
	}
};

struct KeyboardRegister {
	int id;
	std::function<void(int, LPARAM)> callback;
	std::map<int, std::vector<int>>  keyBindings;
};