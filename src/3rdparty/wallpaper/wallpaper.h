#pragma once

#include "../dd_dll_global.h"

#include <Windows.h>

namespace Wallpaper
{

HWND DD_SHARED_EXPORT getWorkerW();
HWND DD_SHARED_EXPORT getProgman();
HWND DD_SHARED_EXPORT getDesktop(bool legacyMode);
bool DD_SHARED_EXPORT setWallpaper(HWND window, bool legacyMode);

}
