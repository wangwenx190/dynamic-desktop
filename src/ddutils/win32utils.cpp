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

}
