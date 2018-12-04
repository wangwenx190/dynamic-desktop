#include "win32utils.h"

#include <wtsapi32.h>
#include <userenv.h>
#include <tlhelp32.h>
#include <tchar.h>

namespace Win32Utils
{

bool isSession1Process()
{
    DWORD processId = GetCurrentProcessId();
    DWORD sessionId;
    if (ProcessIdToSessionId(processId, &sessionId) != TRUE)
        return false;
    return (sessionId == DWORD(1));
}

bool launchSession1Process(LPCTSTR path, LPCTSTR params)
{
    size_t sizeW = _tcslen(path) + 1;
    auto dir = new TCHAR[sizeW];
    _tcscpy(dir, path);
    for (;(dir[sizeW] != '\\') && (sizeW != 0); --sizeW)
        dir[sizeW] = 0;
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { nullptr };
    si.cb = sizeof(si);
    DWORD dwSessionID = WTSGetActiveConsoleSessionId();
    HANDLE hToken = nullptr;
    WTSQueryUserToken(dwSessionID, &hToken);
    HANDLE hDuplicatedToken = nullptr;
    DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, nullptr, SecurityIdentification, TokenPrimary, &hDuplicatedToken);
    LPVOID lpEnvironment = nullptr;
    CreateEnvironmentBlock(&lpEnvironment, hDuplicatedToken, FALSE);
    CreateProcessAsUser(hDuplicatedToken, path, const_cast<LPTSTR>(params), nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT, lpEnvironment, dir, &si, &pi);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    delete [] dir;
    return true;
}

bool isAutoStartServiceInstalled(LPCTSTR name)
{
    bool result = false;
    SC_HANDLE hSCM = OpenSCManager(nullptr, nullptr, 0);
    if (hSCM != nullptr)
    {
        SC_HANDLE hService = OpenService(hSCM, name, SERVICE_QUERY_CONFIG);
        if (hService != nullptr)
        {
            result = true;
            CloseServiceHandle(hService);
        }
        CloseServiceHandle(hSCM);
    }
    return result;
}

void getCurrentDir(LPTSTR path)
{
    if (path == nullptr)
        return;
    TCHAR filePath[MAX_PATH + 1] = { 0 };
    DWORD dwSize = GetModuleFileName(nullptr, filePath, MAX_PATH);
    for (;(filePath[dwSize] != '\\') && (dwSize != 0); --dwSize)
        filePath[dwSize] = 0;
    _tcscpy(path, filePath);
}

// http://undoc.airesoft.co.uk/user32.dll/SetWindowCompositionAttribute.php
void enableBlurBehindWindow(HWND window)
{
    if (window == nullptr)
        return;
    using DD_WINDOWCOMPOSITIONATTRIB = enum _DD_WINDOWCOMPOSITIONATTRIB
    {
        WCA_UNDEFINED = 0,
        WCA_NCRENDERING_ENABLED = 1,
        WCA_NCRENDERING_POLICY = 2,
        WCA_TRANSITIONS_FORCEDISABLED = 3,
        WCA_ALLOW_NCPAINT = 4,
        WCA_CAPTION_BUTTON_BOUNDS = 5,
        WCA_NONCLIENT_RTL_LAYOUT = 6,
        WCA_FORCE_ICONIC_REPRESENTATION = 7,
        WCA_EXTENDED_FRAME_BOUNDS = 8,
        WCA_HAS_ICONIC_BITMAP = 9,
        WCA_THEME_ATTRIBUTES = 10,
        WCA_NCRENDERING_EXILED = 11,
        WCA_NCADORNMENTINFO = 12,
        WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
        WCA_VIDEO_OVERLAY_ACTIVE = 14,
        WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
        WCA_DISALLOW_PEEK = 16,
        WCA_CLOAK = 17,
        WCA_CLOAKED = 18,
        WCA_ACCENT_POLICY = 19,
        WCA_FREEZE_REPRESENTATION = 20,
        WCA_EVER_UNCLOAKED = 21,
        WCA_VISUAL_OWNER = 22,
        WCA_LAST = 23
    };
    using DD_WINDOWCOMPOSITIONATTRIBDATA = struct _DD_WINDOWCOMPOSITIONATTRIBDATA
    {
        DD_WINDOWCOMPOSITIONATTRIB dwAttrib;
        PVOID pvData;
        SIZE_T cbData;
    };
    using DD_ACCENT_STATE = enum _DD_ACCENT_STATE
    {
        ACCENT_DISABLED = 0,
        ACCENT_ENABLE_GRADIENT = 1,
        ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
        ACCENT_ENABLE_BLURBEHIND = 3,
        ACCENT_INVALID_STATE = 4
    };
    using DD_ACCENT_POLICY = struct _DD_ACCENT_POLICY
    {
        DD_ACCENT_STATE AccentState;
        DWORD AccentFlags;
        DWORD GradientColor;
        DWORD AnimationId;
    };
    HMODULE user32Lib = GetModuleHandle(TEXT("User32"));
    if (user32Lib != nullptr)
    {
        using pfnSetWindowCompositionAttribute = BOOL (*)(HWND, DD_WINDOWCOMPOSITIONATTRIBDATA *);
        auto setWindowCompositionAttribute = reinterpret_cast<pfnSetWindowCompositionAttribute>(GetProcAddress(user32Lib, "SetWindowCompositionAttribute"));
        if (setWindowCompositionAttribute != nullptr)
        {
            DD_ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
            DD_WINDOWCOMPOSITIONATTRIBDATA data;
            data.dwAttrib = WCA_ACCENT_POLICY;
            data.pvData = &accent;
            data.cbData = sizeof(accent);
            setWindowCompositionAttribute(window, &data);
        }
    }
}

}
