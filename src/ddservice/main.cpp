#include <Windows.h>
#include <tchar.h>

#include <Win32Utils>

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceHandler(DWORD control);
VOID Install();
VOID Uninstall();
VOID RunDDMain();

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;

int _tmain(int argc, TCHAR *argv[])
{
    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        {const_cast<TCHAR *>(_T("ddassvc")), (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {nullptr, nullptr}
    };
    for (unsigned int i = 1; i != argc; ++i)
        if ((_tcscmp(argv[i], _T("-i")) == 0) || (_tcscmp(argv[i], _T("-I")) == 0) || (_tcscmp(argv[i], _T("-install")) == 0) || (_tcscmp(argv[i], _T("-INSTALL")) == 0) || (_tcscmp(argv[i], _T("-Install")) == 0))
        {
            Install();
            break;
        }
        else if ((_tcscmp(argv[i], _T("-u")) == 0) || (_tcscmp(argv[i], _T("-U")) == 0) || (_tcscmp(argv[i], _T("-uninstall")) == 0) || (_tcscmp(argv[i], _T("-UNINSTALL")) == 0) || (_tcscmp(argv[i], _T("-Uninstall")) == 0))
        {
            Uninstall();
            break;
        }
        else
            break;
    return 0;
}

VOID Install()
{
    TCHAR filePath[MAX_PATH + 1];
    DWORD dwSize = GetModuleFileName(nullptr, filePath, MAX_PATH);
    filePath[dwSize] = 0;
    Win32Utils::installAutoStartService(filePath);
}

VOID Uninstall()
{
    Win32Utils::uninstallAutoStartService();
}

VOID RunDDMain()
{
    auto ddmainDir = new TCHAR[MAX_PATH];
    memset(ddmainDir, 0x00, sizeof(ddmainDir));
    DWORD dwSize = GetModuleFileName(nullptr, ddmainDir, MAX_PATH);
    ddmainDir[dwSize] = 0;
    while (ddmainDir[dwSize] != '\\' && dwSize != 0)
    {
        ddmainDir[dwSize] = 0;
        --dwSize;
    }
    if ((dwSize - 1) > 0)
        ddmainDir[dwSize - 1] = 0;
    auto ddmainPath = (TCHAR*)malloc(MAX_PATH);
    memset(ddmainPath, 0x00, MAX_PATH);
    _tcscpy(ddmainPath, ddmainDir);
#ifdef _DEBUG
    _tcscat(ddmainPath, _T("\\ddmaind.exe"));
#else
    _tcscat(ddmainPath, _T("\\ddmain.exe"));
#endif
    Win32Utils::launchSession1Process(ddmainPath, nullptr, ddmainDir);
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
    (void)argc;
    (void)argv;
    serviceStatus.dwServiceType = SERVICE_WIN32;
    serviceStatus.dwCurrentState = SERVICE_START_PENDING;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 0;
    serviceStatusHandle = RegisterServiceCtrlHandler(_T("ddassvc"), ServiceHandler);
    if (serviceStatusHandle != nullptr)
        return;
    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 0;
    if (!SetServiceStatus(serviceStatusHandle, &serviceStatus))
        return;
    RunDDMain();
}

VOID WINAPI ServiceHandler(DWORD control)
{
    switch (control)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        serviceStatus.dwWin32ExitCode = 0;
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        serviceStatus.dwCheckPoint = 0;
        serviceStatus.dwWaitHint = 0;
        break;
    case SERVICE_CONTROL_PAUSE:
        serviceStatus.dwCurrentState = SERVICE_PAUSED;
        break;
    case SERVICE_CONTROL_CONTINUE:
        serviceStatus.dwCurrentState = SERVICE_RUNNING;
        break;
    case SERVICE_CONTROL_INTERROGATE:
        break;
    default:
        break;
    };
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}
