#pragma once

#include "../dd_dll_global.h"

#include <Windows.h>

namespace Wallpaper
{

void DD_SHARED_EXPORT setLegacyMode(bool legacy = false);
HWND DD_SHARED_EXPORT getProgman();
HWND DD_SHARED_EXPORT getDesktop();
bool DD_SHARED_EXPORT setWallpaper(HWND window);
void DD_SHARED_EXPORT hideWallpaper();
void DD_SHARED_EXPORT showWallpaper();
HWND DD_SHARED_EXPORT getWallpaper();
bool DD_SHARED_EXPORT setWallpaperVisible(bool visible = true);
bool DD_SHARED_EXPORT isWallpaperVisible();
bool DD_SHARED_EXPORT isWallpaperHidden();

}
