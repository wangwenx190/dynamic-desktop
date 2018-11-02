#pragma once

#include "dll_global.h"

namespace Win32Utils
{

#ifdef UNICODE
    #define launchSession1Process launchSession1ProcessW
    #define isAutoStartServiceInstalled isAutoStartServiceInstalledW
#else
    #define launchSession1Process launchSession1ProcessA
    #define isAutoStartServiceInstalled isAutoStartServiceInstalledA
#endif

bool DD_SHARED_EXPORT launchSession1ProcessA(const char *path, const char *params, const char *dir);
bool DD_SHARED_EXPORT launchSession1ProcessW(const wchar_t *path, const wchar_t *params, const wchar_t *dir);
bool DD_SHARED_EXPORT isAutoStartServiceInstalledA(const char *name);
bool DD_SHARED_EXPORT isAutoStartServiceInstalledW(const wchar_t *name);
bool DD_SHARED_EXPORT isSession1Process();

}
