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

}

Setting::~Setting()
{
}

void Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//输入框绑定
	//快捷键绑定系统
	DDX_Control(pDX, IDC_HOTKEY1, HOTKEY1);
	DDX_Control(pDX, IDC_HOTKEY2, HOTKEY2);
	DDX_Control(pDX, IDC_HOTKEY3, HOTKEY3);
	DDX_Control(pDX, IDC_HOTKEY4, HOTKEY4);
	DDX_Control(pDX, IDC_HOTKEY5, HOTKEY5);
	DDX_Control(pDX, IDC_HOTKEY6, HOTKEY6);
	DDX_Control(pDX, IDC_HOTKEY7, HOTKEY7);
	DDX_Control(pDX, IDC_HOTKEY8, HOTKEY8);
	DDX_Control(pDX, IDC_COMBO1, CComboBox_List);

	DDX_Control(pDX, IDC_EDIT1, EDIT);
	DDX_Control(pDX, IDC_EDIT2, EDIT2);
	//设置快捷键显示
	if (info != NULL)
	{
		SetKeyShow(HOTKEY1, info->QuickKey[1]);
		std::vector<int>list = info->QuickKey[2];
		list.pop_back();
		SetKeyShow(HOTKEY2, list);
		list = info->QuickKey[3];
		list.pop_back();
		SetKeyShow(HOTKEY3, list);
		SetKeyShow(HOTKEY4, info->QuickKey[4]);
		SetKeyShow(HOTKEY5, info->QuickKey[5]);
		SetKeyShow(HOTKEY6, info->QuickKey[6]);
		SetKeyShow(HOTKEY7, info->QuickKey[7]);
		SetKeyShow(HOTKEY8, info->QuickKey[8]);

		for (int x = 0; x<info->POINT_100M.size(); x++)
		{
			if (x == 0)
				CComboBox_List.SetCurSel(0);
			std::wstring str = L"放大";
			str += std::to_wstring(x);
			str += L"次";
			CComboBox_List.AddString(str.c_str());
		}
		
		EDIT2.SetWindowTextW(std::to_wstring(info->PointSize).c_str());

	}


	
}



BEGIN_MESSAGE_MAP(Setting, CDialogEx)
	ON_BN_CLICKED(IDOK, &Setting::OnBnClickedOk)

	ON_CBN_SELCHANGE(IDC_COMBO1, &Setting::OnCbnSelchangeCombo1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &Setting::OnEnKillfocusEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &Setting::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Setting::OnBnClickedButton2)
END_MESSAGE_MAP()


// Setting 消息处理程序


void Setting::OnBnClickedOk()
{
	if (info!=nullptr)
	{
		SetKeyQuick();
		CString str;
		EDIT2.GetWindowTextW(str);
		info->PointSize = _ttoi(str);
		MainWindow::SaveConfigurationBinary("1");
	}
	
}

void Setting::Setinfo(MainWindowInfo &showinfo)
{
	info = &showinfo;
	if (info->QuickKey.empty())
	{
		return;
	}
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
	info->QuickKey[1] = GetkeyList(HOTKEY1);
	std::vector<int> list = GetkeyList(HOTKEY2);
	list.push_back(0x01);
	info->QuickKey[2] = list;
	list =  GetkeyList(HOTKEY3);
	list.push_back(0x01);
	info->QuickKey[3] = list;
	info->QuickKey[4] = GetkeyList(HOTKEY4);
	info->QuickKey[5] = GetkeyList(HOTKEY5);
	info->QuickKey[6] = GetkeyList(HOTKEY6);
	info->QuickKey[7] = GetkeyList(HOTKEY7);
	info->QuickKey[8] = GetkeyList(HOTKEY8);

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

void Setting::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = CComboBox_List.GetCurSel();
	std::cout << nSel << std::endl;
	
	EDIT.SetWindowTextW(std::to_wstring(info->POINT_100M[nSel]).c_str());
}

void Setting::OnEnKillfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (info->POINT_100M.size()==0||CComboBox_List.GetCurSel()== CB_ERR)
	{
		return;
	}
	CString str;
	EDIT.GetWindowTextW(str);
	info->POINT_100M[CComboBox_List.GetCurSel()] = _ttoi(str);
}

void Setting::OnBnClickedButton1()
{
	info->POINT_100M.push_back(0);
	std::wstring str = L"放大";
	str += std::to_wstring(info->POINT_100M.size() - 1);
	str += L"次";
	CComboBox_List.AddString(str.c_str());
	// TODO: 在此添加控件通知处理程序代码
}

void Setting::OnBnClickedButton2()
{

	int itemCount = CComboBox_List.GetCount(); // 获取组合框中的项数

	if (itemCount!=0)
	{
		CComboBox_List.DeleteString(itemCount - 1);
		CComboBox_List.SetCurSel(itemCount - 2);
		info->POINT_100M.pop_back();
		if (itemCount>1)
		{
			OnCbnSelchangeCombo1();
		}
		
	}
}
