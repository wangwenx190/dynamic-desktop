#include <Windows.h>
#include <tchar.h>

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceHandler(DWORD control);
VOID Install();
VOID Uninstall();

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
        if ((_tcscmp(argv[i], _T("-i")) == 0) || (_tcscmp(argv[i], _T("-I")) == 0))
        {
            Install();
            break;
        }
        else if ((_tcscmp(argv[i], _T("-u")) == 0) || (_tcscmp(argv[i], _T("-U")) == 0))
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
    filePath[dwSize] = '\0';
    SC_HANDLE manager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
    if (manager != nullptr)
    {
        SC_HANDLE service = CreateService(manager, _T("ddassvc"), _T("Dynamic Desktop Auto Start Service"), SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, filePath, nullptr, nullptr, nullptr, nullptr, nullptr);
        if (service != nullptr)
            CloseServiceHandle(service);
        CloseServiceHandle(manager);
    }
}

VOID Uninstall()
{
    SC_HANDLE manager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (manager != nullptr)
    {
        SC_HANDLE service = OpenService(manager, _T("ddassvc"), SERVICE_ALL_ACCESS);
        if (service != nullptr)
        {
            DeleteService(service);
            CloseServiceHandle(service);
        }
        CloseServiceHandle(manager);
    }
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
    if (!SetServiceStatus(serviceStatusHandle, &serviceStatus))
        return;
}
