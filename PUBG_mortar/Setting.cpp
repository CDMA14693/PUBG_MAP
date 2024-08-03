// Setting.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "Setting.h"
#include "HookHandler.h"
#include "MainWindow.h"

// Setting 对话框

IMPLEMENT_DYNAMIC(Setting, CDialogEx)


Setting::Setting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	hook = &(HookHandler::GetHookHandler());
}

Setting::~Setting()
{
}

void Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//输入框绑定
	DDX_Control(pDX, IDC_EDIT1, M100_EDIT);
	//快捷键绑定系统
	DDX_Control(pDX, IDC_HOTKEY1, HOTKEY1);
	DDX_Control(pDX, IDC_HOTKEY2, HOTKEY2);
	DDX_Control(pDX, IDC_HOTKEY3, HOTKEY3);
	DDX_Control(pDX, IDC_HOTKEY5, HOTKEY4);
	DDX_Control(pDX, IDC_HOTKEY6, HOTKEY5);
	DDX_Control(pDX, IDC_HOTKEY7, HOTKEY6);
	DDX_Control(pDX, IDC_HOTKEY8, HOTKEY7);
	//设置快捷键显示
	SetKeyShow(HOTKEY1, HookHandler::QuickKey["渲染标记点"]);
	SetKeyShow(HOTKEY2, HookHandler::QuickKey["标记点1"]);
	SetKeyShow(HOTKEY3, HookHandler::QuickKey["标记点2"]);
	SetKeyShow(HOTKEY4, HookHandler::QuickKey["增加基准值"]);
	SetKeyShow(HOTKEY5, HookHandler::QuickKey["减少基准值"]);
	SetKeyShow(HOTKEY6, HookHandler::QuickKey["标记中心点1"]);
	SetKeyShow(HOTKEY7, HookHandler::QuickKey["标记中心点2"]);
	//设置基准值显示
	M100_EDIT.SetWindowTextW(std::to_wstring(MainWindow::POINT_100M).c_str());
}



BEGIN_MESSAGE_MAP(Setting, CDialogEx)
	ON_BN_CLICKED(IDOK, &Setting::OnBnClickedOk)

	ON_EN_CHANGE(IDC_EDIT1, &Setting::OnEnChangeEdit1)
END_MESSAGE_MAP()


// Setting 消息处理程序


void Setting::OnBnClickedOk()
{
	SetKeyQuick();



}

//设置窗口快捷键显示
bool Setting::SetKeyShow(CHotKeyCtrl& HOYKEY, std::vector<int>& keyvalue)
{
	if (keyvalue.empty()) return false;

	WORD wVirtualKeyCode = 0;
	WORD wModifiers = 0;

	// Check if the first value in keyvalue indicates a modifier key
	if (keyvalue.size() > 1) {
		// Determine modifiers
		if (keyvalue.at(0) == 0x11) { // Ctrl
			wModifiers |= HOTKEYF_CONTROL;
		}
		else if (keyvalue.at(0) == 0xA2 || keyvalue.at(0) == 0xA3) { // Right or Left Ctrl
			wModifiers |= HOTKEYF_CONTROL;
		}
		else if (keyvalue.at(0) == 0x10) { // Shift
			wModifiers |= HOTKEYF_SHIFT;
		}
		else if (keyvalue.at(0) == 0x12) { // Alt
			wModifiers |= HOTKEYF_ALT;
		}

		// The second value is the actual virtual key code
		wVirtualKeyCode = keyvalue.at(1);
	}
	else {
		// If there's only one key value, it's treated as a single key hotkey
		wVirtualKeyCode = keyvalue.at(0);
	}

	HOYKEY.SetHotKey(wVirtualKeyCode, wModifiers);
	return true;
}

//将窗口快捷键设置给快捷键处理程序
bool Setting::SetKeyQuick()
{

	HookHandler::QuickKey["渲染标记点"] = GetkeyList(HOTKEY1);
	HookHandler::QuickKey["标记点1"] = GetkeyList(HOTKEY2);
	HookHandler::QuickKey["标记点2"] = GetkeyList(HOTKEY3);
	HookHandler::QuickKey["增加基准值"] = GetkeyList(HOTKEY4);
	HookHandler::QuickKey["减少基准值"] = GetkeyList(HOTKEY5);
	HookHandler::QuickKey["标记中心点1"] = GetkeyList(HOTKEY6);
	HookHandler::QuickKey["标记中心点2"] = GetkeyList(HOTKEY7);

	CString str;
	M100_EDIT.GetWindowTextW(str);
	if(_ttoi(str)>0)
		MainWindow::POINT_100M = _ttoi(str);
	hook->SetUpdateDraw(true);


	return false;
}

//将窗口快捷键转换为 键值顺序
std::vector<int> Setting::GetkeyList(CHotKeyCtrl& HOTKEY)
{
	std::vector<int> key;
	WORD wVirtualKeyCode = 0;
	WORD wModifiers = 0;

	// 使用正确的重载版本来获取热键
	HOTKEY.GetHotKey(wVirtualKeyCode, wModifiers);

	if (wModifiers & HOTKEYF_SHIFT) key.push_back(0xA0);
	if (wModifiers & HOTKEYF_CONTROL)key.push_back(0xA2);
	if (wModifiers & HOTKEYF_ALT)key.push_back(0xA4);

	key.push_back(wVirtualKeyCode);
	return key;
}



void Setting::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	
}
