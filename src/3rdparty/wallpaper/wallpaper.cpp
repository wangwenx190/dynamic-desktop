#include "wallpaper.h"

namespace Wallpaper
{

static HWND HWORKERW = nullptr;

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
    (void)lParam;
    HWND defview = FindWindowEx(hwnd, nullptr, TEXT("SHELLDLL_DefView"), nullptr);
    if (defview != nullptr)
    {
        HWORKERW = FindWindowEx(nullptr, hwnd, TEXT("WorkerW"), nullptr);
        if (HWORKERW != nullptr)
            return FALSE;
    }
    return TRUE;
}

HWND getProgman()
{
    return FindWindow(TEXT("Progman"), TEXT("Program Manager"));
}

HWND getDesktop(bool legacyMode)
{
    HWND hwnd = getProgman();
    SendMessage(hwnd, 0x052c, 0, 0);
    EnumWindows(EnumWindowsProc, 0);
    ShowWindow(HWORKERW, legacyMode ? SW_HIDE : SW_SHOW);
    return legacyMode ? hwnd : HWORKERW;
}

HWND getWorkerW()
{
    return HWORKERW;
}

bool setWallpaper(HWND window, bool legacyMode)
{
    if (window == nullptr)
        return false;
    HWND desktop = getDesktop(legacyMode);
    HWND parent = nullptr;
    if (desktop != nullptr)
        parent = SetParent(window, desktop);
    return parent == nullptr ? false : true;
}

}
