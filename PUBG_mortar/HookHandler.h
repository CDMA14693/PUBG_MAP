#pragma once

//struct KeyboardRegister {
//	int id;
//	std::function<void(int,LPARAM)> callback;
//	std::map<int, std::vector<int>>  keyBindings;
//};


class HookHandler
{
public:
	//唯一单例 创造与返回
	static HookHandler& GetHookHandler();
	static LRESULT CALLBACK ALLMessageProc(int nCode, WPARAM wParam, LPARAM lParam);

	bool installHook(HOOKPROC proc = ALLMessageProc);
	void UnistallHook() const;
	//注册快捷键 以及处理快捷键函数 
	std::vector<KeyboardRegister>::iterator RegisterKeyboard(std::function<void(int,LPARAM)>, std::map<int, std::vector<int>>);

	bool RemoveRegisterKeyboard(std::vector<KeyboardRegister>::iterator it);


private:
	HookHandler();
	HHOOK MouseHook=nullptr;
	HHOOK KeyboardHook=nullptr;

	//当前按键列表
	static std::vector<KeyboardRegister> keyRegisters;

	
	//比较按键值
	static void compareKeyList(const std::vector<int>& b,LPARAM lparm);
	//删除当前抬起的 按键  
	static void removeKey(std::vector<int>& keyValue, int valueToRemove);
};
