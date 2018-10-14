#pragma once

#include "wallpaper_global.h"

#include <Windows.h>

namespace Wallpaper
{

WALLPAPERSHARED_EXPORT HWND getWorkerW();
WALLPAPERSHARED_EXPORT HWND getProgman();
WALLPAPERSHARED_EXPORT HWND getDesktop(bool legacyMode);
WALLPAPERSHARED_EXPORT bool setWallpaper(HWND window, bool legacyMode);

}
