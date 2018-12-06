#pragma once

#include "win32utils_global.h"

#include <Windows.h>

namespace Win32Utils
{

#ifdef __cplusplus
extern "C" {
#endif

bool DD_SHARED_EXPORT launchSession1Process(LPCTSTR path, LPCTSTR params);
bool DD_SHARED_EXPORT isAutoStartServiceInstalled(LPCTSTR name);
bool DD_SHARED_EXPORT isSession1Process();
bool DD_SHARED_EXPORT getCurrentDir(LPTSTR path);
bool DD_SHARED_EXPORT enableBlurBehindWindow(HWND window);

#ifdef __cplusplus
}
#endif

}
