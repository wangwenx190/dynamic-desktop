#include "wallpaper.h"

namespace Wallpaper
{

bool legacyMode = false;
HWND HWORKERW = nullptr;

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

HWND getDesktop()
{
    HWND hwnd = getProgman();
    SendMessage(hwnd, 0x052c, 0, 0);
    EnumWindows(EnumWindowsProc, 0);
    ShowWindow(HWORKERW, legacyMode ? SW_HIDE : SW_SHOW);
    return legacyMode ? hwnd : HWORKERW;
}

bool setWallpaper(HWND window)
{
    if (window == nullptr)
        return false;
    HWND desktop = getDesktop();
    HWND parent = nullptr;
    if (desktop != nullptr)
        parent = SetParent(window, desktop);
    return parent == nullptr ? false : true;
}

void hideWallpaper()
{
    if (getWallpaper() != nullptr)
        setWallpaperVisible(false);
}

HWND getWallpaper()
{
    return legacyMode ? getProgman() : HWORKERW;
}

void showWallpaper()
{
    if (getWallpaper() != nullptr)
        setWallpaperVisible(true);
}

bool isWallpaperVisible()
{
    return getWallpaper() == nullptr ? false : IsWindowVisible(getWallpaper());
}

bool isWallpaperHidden()
{
    return !isWallpaperVisible();
}

bool setWallpaperVisible(bool visible)
{
    if (getWallpaper() == nullptr)
        return false;
    if (visible != isWallpaperVisible())
        ShowWindow(getWallpaper(), visible ? SW_SHOW : SW_HIDE);
}

void setLegacyMode(bool legacy)
{
    if (legacyMode != legacy)
        legacyMode = legacy;
}

}
