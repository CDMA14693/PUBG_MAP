#include "pch.h"
#include "HookHandler.h"
#include "MainWindow.h"

//快捷键 功能启动标记说明
//鼠标功能  
std::vector<POINT>* HookHandler::MainWindowPoint = &MainWindow::GetPoint();
std::map<std::string, std::vector<int>> HookHandler::QuickKey;
static BOOL upMainWindow;
static std::vector<int>  KeyValue;

BOOL HookHandler::GetUpdateDraw()
{
    return upMainWindow;
}

void HookHandler::SetUpdateDraw(BOOL flag)
{
    upMainWindow = flag;
}

HookHandler::HookHandler()
{
    upMainWindow = false;
}


HookHandler& HookHandler::GetHookHandler()
{
    static HookHandler myHookHandler;
    return myHookHandler;
}

LRESULT HookHandler::ALLMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    
    if (nCode >= 0) {
        if (wParam==WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT* keycode = (KBDLLHOOKSTRUCT*)lParam;
            if (KeyValue.empty())
            {
                KeyValue.push_back(keycode->vkCode);
            }
            else if (KeyValue.back()!= keycode->vkCode)
            {
                KeyValue.push_back(keycode->vkCode);
            }            
        }
        else if (wParam == WM_LBUTTONDOWN )
        {
            MSLLHOOKSTRUCT* mouseCode = (MSLLHOOKSTRUCT*)lParam;
            if ((!KeyValue.empty())&& !QuickKey.empty()&&!MainWindowPoint->empty())
            {
                if (KeyValue == QuickKey["标记点1"])
                {
                    MainWindowPoint->at(0) = mouseCode->pt;
                }
                else if (KeyValue == QuickKey["标记点2"]) 
                {
                    MainWindowPoint->at(1) = mouseCode->pt;
                }
                upMainWindow = true;
            }
            
        }
        //按键抬起后的 组合键处理
        else if (wParam== WM_KEYUP)
        {
            
            


            KBDLLHOOKSTRUCT* keycode = (KBDLLHOOKSTRUCT*)lParam;
            if (isKey(QuickKey["渲染标记点"], KeyValue))
            {
                MainWindow::DrawPoint = !MainWindow::DrawPoint;
            }
            else if (isKey(KeyValue,QuickKey["标记中心点1"]))
            {

                std::cout << "标记中心点1"<<std::endl;
                std::cout << "宽" << MainWindow::WindowSize.right;
                std::cout << "高" << MainWindow::WindowSize.bottom;
                MainWindowPoint->at(0).x = MainWindow::WindowSize.right / 2;
                MainWindowPoint->at(0).y = MainWindow::WindowSize.bottom / 2;
            }
            else if (isKey(KeyValue, QuickKey["标记中心点2"]))
            {
                MainWindowPoint->at(1).x = MainWindow::WindowSize.right / 2;
                MainWindowPoint->at(1).y = MainWindow::WindowSize.bottom / 2;
            }
            else if (isKey(KeyValue , QuickKey["增加基准值"]))
            {
                MainWindow::POINT_100M += 5;
            }
            else if (KeyValue == QuickKey["减少基准值"])
            {
                MainWindow::POINT_100M -= 5;
            }

            //删除抬起键值
            removeKeyValue(KeyValue, keycode->vkCode);

            //设置画面开始更新
            upMainWindow = true;
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
    return true;
}

void HookHandler::UnistallHook()
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


void HookHandler::removeKeyValue(std::vector<int>& keyValue, int valueToRemove)
{

    // 使用 std::remove 将所有 valueToRemove 移到序列末尾，并返回新末尾的迭代器
    auto newEnd = std::remove(keyValue.begin(), keyValue.end(), valueToRemove);

    // 使用 erase 删除从 newEnd 到 end 的所有元素
    keyValue.erase(newEnd, keyValue.end());
}

BOOL HookHandler::isKey(std::vector<int>& a, std::vector<int>& b)
{
    if (!a.empty() && (a.size()==b.size()))
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            if (a.at(i) != b.at(i))
            {
                return false;
            }
            
        }
        return true;
    }

    return false;
}

std::vector<int> HookHandler::GetNowKey()
{
    return KeyValue;
}

const std::map<std::string, std::vector<int>> HookHandler::GetQuickKey()
{
    return QuickKey;
}

BOOL HookHandler::SetQuickKey(std::map<std::string, std::vector<int>>& tempQuickKey)
{
    std::cout << "设置快捷键" << std::endl;
    if (!tempQuickKey.empty())
    {
        QuickKey = tempQuickKey;
        return true;
    }
    return false;
}

