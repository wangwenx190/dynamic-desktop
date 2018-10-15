#pragma once

#include "../dd_dll_global.h"

#include <Windows.h>

namespace Wallpaper
{

DD_SHARED_EXPORT HWND getWorkerW();
DD_SHARED_EXPORT HWND getProgman();
DD_SHARED_EXPORT HWND getDesktop(bool legacyMode);
DD_SHARED_EXPORT bool setWallpaper(HWND window, bool legacyMode);

}
