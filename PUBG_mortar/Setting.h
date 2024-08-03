#pragma once
#include "afxdialogex.h"
#include "HookHandler.h"

// Setting 对话框

class Setting : public CDialogEx
{
	DECLARE_DYNAMIC(Setting)

public:
	Setting(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Setting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CHotKeyCtrl HOTKEY1;



	
	//设置窗口快捷键显示
	bool SetKeyShow(CHotKeyCtrl&, std::vector<int>&);
	//将窗口快捷键设置给快捷键处理程序
	bool SetKeyQuick();
	//将窗口快捷键转换为 键值顺序
	std::vector<int> GetkeyList(CHotKeyCtrl&);
	afx_msg void OnEnChangeEdit1();
	HookHandler* hook;
	CEdit M100_EDIT;
	CHotKeyCtrl HOTKEY2;
	CHotKeyCtrl HOTKEY3;
	CHotKeyCtrl HOTKEY4;
	CHotKeyCtrl HOTKEY5;
	CHotKeyCtrl HOTKEY6;
	CHotKeyCtrl HOTKEY7;
};
