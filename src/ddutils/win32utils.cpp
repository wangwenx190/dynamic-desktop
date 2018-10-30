#include "win32utils.h"

#include <Windows.h>
#include <wtsapi32.h>
#include <userenv.h>
#include <tlhelp32.h>
#include <tchar.h>

namespace Win32Utils
{

wchar_t *toW(const char *from)
{
    size_t newSizeW = strlen(from) + 1;
    auto resultW = new wchar_t[newSizeW];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, resultW, newSizeW, from, _TRUNCATE);
    return resultW;
}

bool launchSession1ProcessA(const char *path, const char *params, const char *dir)
{
    wchar_t *pathW = toW(path);
    wchar_t *paramsW = params == nullptr ? nullptr : toW(params);
    wchar_t *dirW = dir == nullptr ? nullptr : toW(dir);
    bool result = launchSession1ProcessW(pathW, paramsW, dirW);
    delete [] pathW;
    delete [] paramsW;
    delete [] dirW;
    return result;
}

bool isAutoStartServiceInstalled()
{
    bool result = false;
    SC_HANDLE hSCM = OpenSCManagerW(nullptr, nullptr, 0);
    if (hSCM != nullptr)
    {
        SC_HANDLE hService = OpenServiceW(hSCM, _T("ddassvc"), SERVICE_QUERY_CONFIG);
        if (hService != nullptr)
        {
            result = true;
            CloseServiceHandle(hService);
        }
        CloseServiceHandle(hSCM);
    }
    return result;
}

bool isSession1Process()
{
    DWORD processId = GetCurrentProcessId();
    DWORD sessionId;
    if (ProcessIdToSessionId(processId, &sessionId) != TRUE)
        return false;
    return (sessionId == DWORD(1));
}

bool launchSession1ProcessW(const wchar_t *path, const wchar_t *params, const wchar_t *dir)
{
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { nullptr };
    si.cb = sizeof(si);
    DWORD dwSessionID = WTSGetActiveConsoleSessionId();
    HANDLE hToken = nullptr;
    if (WTSQueryUserToken(dwSessionID, &hToken) != TRUE)
        return false;
    HANDLE hDuplicatedToken = nullptr;
    if (DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, nullptr, SecurityIdentification, TokenPrimary, &hDuplicatedToken) != TRUE)
        return false;
    LPVOID lpEnvironment = nullptr;
    if (CreateEnvironmentBlock(&lpEnvironment, hDuplicatedToken, FALSE) != TRUE)
        return false;
    if (CreateProcessAsUserW(hDuplicatedToken, path, const_cast<wchar_t *>(params), nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT, lpEnvironment, dir, &si, &pi) != TRUE)
        return false;
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

}
