#pragma once

#include <Windows.h>

namespace Wallpaper
{

void setLegacyMode(bool legacy = false);
HWND getProgman();
HWND getDesktop();
bool setWallpaper(HWND window);
void hideWallpaper();
void showWallpaper();
HWND getWallpaper();
bool setWallpaperVisible(bool visible = true);
bool isWallpaperVisible();
bool isWallpaperHidden();

}
