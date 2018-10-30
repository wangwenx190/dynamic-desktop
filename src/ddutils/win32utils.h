#pragma once

#include "dll_global.h"

namespace Win32Utils
{

#if defined(UNICODE) || defined(_UNICODE)
    #define launchSession1Process launchSession1ProcessW
    #define installAutoStartService installAutoStartServiceW
#else
    #define launchSession1Process launchSession1ProcessA
    #define installAutoStartService installAutoStartServiceA
#endif

bool DD_SHARED_EXPORT launchSession1ProcessA(const char *path, const char *params, const char *dir);
bool DD_SHARED_EXPORT launchSession1ProcessW(const wchar_t *path, const wchar_t *params, const wchar_t *dir);
bool DD_SHARED_EXPORT isAutoStartServiceInstalled();
bool DD_SHARED_EXPORT installAutoStartServiceA(const char *path);
bool DD_SHARED_EXPORT installAutoStartServiceW(const wchar_t *path);
bool DD_SHARED_EXPORT uninstallAutoStartService();
bool DD_SHARED_EXPORT isSession1Process();

}
