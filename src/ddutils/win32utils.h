#pragma once

#include "dll_global.h"

#include <Windows.h>

namespace Win32Utils
{

bool DD_SHARED_EXPORT launchSession1Process(LPCTSTR path, LPCTSTR params);
bool DD_SHARED_EXPORT isAutoStartServiceInstalled(LPCTSTR name);
bool DD_SHARED_EXPORT isSession1Process();

}
