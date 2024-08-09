#include "pch.h"
#include "HookHandler.h"
#include "MainWindow.h"


//快捷键 功能启动标记说明
//鼠标功能  
/*
 鼠标消息定义 int
    鼠标滚轮上 0x1000
    鼠标滚轮下 0x1001

 鼠标左键 
*/


std::vector<KeyboardRegister> HookHandler::keyRegisters;

HookHandler::HookHandler() {

}

HookHandler& HookHandler::GetHookHandler()
{
    static HookHandler myHookHandler;
    return myHookHandler;
}

LRESULT HookHandler::ALLMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    static std::vector<int>  pressedKeys;
    static LPARAM temp;
    if (nCode >= 0&&!keyRegisters.empty()) {
        switch (wParam)
        {
        case WM_KEYDOWN:
        {
            KBDLLHOOKSTRUCT* keycode = (KBDLLHOOKSTRUCT*)lParam;
            if (pressedKeys.empty())
            {
                pressedKeys.push_back(keycode->vkCode);
            }
            else if (pressedKeys.back() != keycode->vkCode)
            {
                pressedKeys.push_back(keycode->vkCode);
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            pressedKeys.push_back(0x01);
            compareKeyList(pressedKeys, lParam);
            removeKey(pressedKeys, 0x01);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            pressedKeys.push_back(0x02);
            compareKeyList(pressedKeys, lParam);
            removeKey(pressedKeys, 0x02);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            MSLLHOOKSTRUCT* s=(MSLLHOOKSTRUCT*)lParam;
            int x = GET_WHEEL_DELTA_WPARAM(s->mouseData) / WHEEL_DELTA;
            if (x>0)
            {
                pressedKeys.push_back(0x1000);
            }
            else if (x < 0)
            {

                pressedKeys.push_back(0x1001);
                
            }
            compareKeyList(pressedKeys, lParam);
            removeKey(pressedKeys, 0x1000);
            removeKey(pressedKeys, 0x1001);
            break;
        }
        case WM_KEYUP:
        {
            KBDLLHOOKSTRUCT* keycode = (KBDLLHOOKSTRUCT*)lParam;
            compareKeyList(pressedKeys, lParam);
            removeKey(pressedKeys, keycode->vkCode);
            break;
        }
        default:
            break;
        }

    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool HookHandler::installHook(HOOKPROC proc)
{
    //安装钩子 ，失败就退出
    MouseHook = SetWindowsHookEx(WH_MOUSE_LL, proc, NULL, 0);
    if (MouseHook==nullptr)
    {
        std::cout << "钩子安装失败" << std::endl;

        return false;
    }
    KeyboardHook=SetWindowsHookEx(WH_KEYBOARD_LL, proc, NULL, 0);
    if (KeyboardHook==nullptr)
    {
        std::cout << "钩子安装失败" << std::endl;
        UnhookWindowsHookEx(MouseHook);
        return false;
    }
    std::cout << "钩子安装成功" << std::endl;
    return true;
}

void HookHandler::UnistallHook() const
{
    if (MouseHook!=nullptr)
    {
        UnhookWindowsHookEx(MouseHook);
    }
    if (KeyboardHook!=nullptr)
    {
        UnhookWindowsHookEx(KeyboardHook);
    }
}



std::vector<KeyboardRegister>::iterator HookHandler::RegisterKeyboard(std::function<void(int,LPARAM)> callback, std::map<int, std::vector<int>> key) {
    static int id = 0;
    KeyboardRegister KeyRegister;
    KeyRegister.callback = callback;
    KeyRegister.keyBindings = key;
    KeyRegister.id = id++;

    return keyRegisters.insert(keyRegisters.end(), KeyRegister);
}

bool HookHandler::RemoveRegisterKeyboard(std::vector<KeyboardRegister>::iterator it)
{
    if (it == keyRegisters.end()) {
        // 迭代器无效，返回 false
        return false;
    }

    keyRegisters.erase(it);
    return true;
}


void HookHandler::compareKeyList(const std::vector<int>& b, LPARAM lparm)
{
    if (!keyRegisters.empty() && !b.empty()) {
        for (const auto& registerInfo : keyRegisters) {
            for (const auto& binding : registerInfo.keyBindings) {
                // 只考虑大小相等的队列
                if (binding.second.size() <= b.size()&& binding.second.size()>0) {
                    bool isMatch = true;
                    size_t matchStartIndex = 0;

                    // 遍历队列，检查是否能找到匹配
                    for (size_t i = 0; i <= b.size() - binding.second.size(); ++i) {
                        isMatch = true;
                        for (size_t j = 0; j < binding.second.size(); ++j) {
                            if (binding.second[j] != b[i + j]) {
                                isMatch = false;
                                break;
                            }
                        }
                        if (isMatch) {
                            matchStartIndex = i;
                            break;
                        }
                    }

                    // 如果找到匹配，执行回调
                    if (isMatch) {
                        registerInfo.callback(binding.first, lparm);
                    }
                }
            }
        }
    }
}

void HookHandler::removeKey(std::vector<int>& keyValue, int valueToRemove)
{

    // 使用 std::remove 将所有 valueToRemove 移到序列末尾，并返回新末尾的迭代器
    auto newEnd = std::remove(keyValue.begin(), keyValue.end(), valueToRemove);

    // 使用 erase 删除从 newEnd 到 end 的所有元素
    keyValue.erase(newEnd, keyValue.end());
}

