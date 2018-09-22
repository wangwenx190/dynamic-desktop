#include "utils.h"

#include <QObject>

HWND workerw = nullptr;

//https://github.com/ThomasHuai/Wallpaper/blob/master/utils.cpp
inline BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lparam)
{
    Q_UNUSED(lparam)
    HWND defview = FindWindowEx(hwnd, nullptr, TEXT("SHELLDLL_DefView"), nullptr);
    if (defview != nullptr)
    {
        workerw = FindWindowEx(nullptr, hwnd, TEXT("WorkerW"), nullptr);
    }
    return TRUE;
}

HWND Utils::getWorkerW()
{
    int result;
    HWND hwnd = FindWindow(TEXT("Progman"), nullptr);
    // 使用 0x3e8 命令分割出两个 WorkerW
    SendMessageTimeout(hwnd, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, reinterpret_cast<PDWORD_PTR>(&result));
    // 遍历窗体获得窗口句柄
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(nullptr));
    // 隐藏WorkerW
    hideWorkerW();
    return hwnd;
}

void Utils::showWorkerW()
{
    ShowWindow(workerw, SW_SHOW);
}

void Utils::hideWorkerW()
{
    ShowWindow(workerw, SW_HIDE);
}
