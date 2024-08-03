#pragma once
struct QuickKeyMod
{
	std::string function;
	std::vector<int> KeyValueList;
};

class HookHandler
{
public:
	//唯一单例 创造与返回
	static HookHandler& GetHookHandler();
	static LRESULT CALLBACK ALLMessageProc(int nCode, WPARAM wParam, LPARAM lParam);

	bool installHook(HOOKPROC proc = ALLMessageProc);
	void UnistallHook();
	
	void updataKey(std::string&);

	//
	static void removeKeyValue(std::vector<int>& keyValue, int valueToRemove);
	static BOOL isKey(std::vector<int>&, std::vector<int>&);
	static std::vector<int> GetNowKey();
	//获取快捷键组合
	static const std::map<std::string, std::vector<int>> GetQuickKey();
	static std::map<std::string, std::vector<int>> QuickKey;
	//设置快捷键组合
	BOOL SetQuickKey(std::map<std::string, std::vector<int>>&);
	HookHandler(const HookHandler&) = delete;
	HookHandler& operator=(HookHandler&) = delete;

	//这是零时搭建用于更新画面的函数，程序主循环会检测它
	BOOL GetUpdateDraw();
	void SetUpdateDraw(BOOL);
private:
	HookHandler();
	HHOOK MouseHook=nullptr;
	HHOOK KeyboardHook=nullptr;
	static std::vector<POINT>* MainWindowPoint;

	
};
