#include <Windows.h>
#include <tchar.h>

#include <Win32Utils>

SERVICE_STATUS serviceStatus = { 0 };
SERVICE_STATUS_HANDLE statusHandle = nullptr;
HANDLE serviceStopEvent = INVALID_HANDLE_VALUE;

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD code);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);

VOID Install();
VOID Uninstall();
VOID RunDDMain();

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
    if (StartServiceCtrlDispatcher(ServiceTable) != TRUE)
        return GetLastError();
    return 0;
}

VOID Install()
{
    TCHAR filePath[MAX_PATH + 1];
    DWORD dwSize = GetModuleFileName(nullptr, filePath, MAX_PATH);
    filePath[dwSize] = 0;
    SC_HANDLE hSCM = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (hSCM != nullptr)
    {
        SC_HANDLE hService = CreateServiceW(hSCM, _T("ddassvc"), _T("Dynamic Desktop Auto Start Service"), SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, filePath, nullptr, nullptr, nullptr, nullptr, nullptr);
        if (hService != nullptr)
        {
            SERVICE_DESCRIPTION sdesc;
            sdesc.lpDescription = const_cast<LPWSTR>(_T("Make Dynamic Desktop automatically run when the system starts. Dynamic Desktop will not auto start if you disabled this service."));
            ChangeServiceConfig2W(hService, SERVICE_CONFIG_DESCRIPTION, &sdesc);
            CloseServiceHandle(hService);
        }
        CloseServiceHandle(hSCM);
    }
}

VOID Uninstall()
{
    SC_HANDLE hSCM = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (hSCM != nullptr)
    {
        SC_HANDLE hService = OpenServiceW(hSCM, _T("ddassvc"), DELETE);
        if (hService != nullptr)
        {
            DeleteService(hService);
            CloseServiceHandle(hService);
        }
        CloseServiceHandle(hSCM);
    }
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
    auto ddmainPath = (TCHAR*)malloc(MAX_PATH);
    memset(ddmainPath, 0x00, MAX_PATH);
    _tcscpy(ddmainPath, ddmainDir);
#ifdef _DEBUG
    _tcscat(ddmainPath, _T("ddmaind.exe"));
#else
    _tcscat(ddmainPath, _T("ddmain.exe"));
#endif
    Win32Utils::launchSession1Process(ddmainPath, nullptr, ddmainDir);
    free(ddmainPath);
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
    (void)argc;
    (void)argv;
    statusHandle = RegisterServiceCtrlHandler(_T("ddassvc"), ServiceCtrlHandler);
    if (statusHandle == nullptr)
        return;
    ZeroMemory(&serviceStatus, sizeof(serviceStatus));
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwControlsAccepted = 0;
    serviceStatus.dwCurrentState = SERVICE_START_PENDING;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    SetServiceStatus(statusHandle, &serviceStatus);
    serviceStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (serviceStopEvent == nullptr)
    {
        serviceStatus.dwControlsAccepted = 0;
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        serviceStatus.dwWin32ExitCode = GetLastError();
        serviceStatus.dwCheckPoint = 1;
        SetServiceStatus(statusHandle, &serviceStatus);
        return;
    }
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    SetServiceStatus(statusHandle, &serviceStatus);
    HANDLE hThread = CreateThread(nullptr, 0, ServiceWorkerThread, nullptr, 0, nullptr);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(serviceStopEvent);
    serviceStatus.dwControlsAccepted = 0;
    serviceStatus.dwCurrentState = SERVICE_STOPPED;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwCheckPoint = 3;
    SetServiceStatus(statusHandle, &serviceStatus);
}

VOID WINAPI ServiceCtrlHandler(DWORD code)
{
    switch (code)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        if (serviceStatus.dwCurrentState != SERVICE_RUNNING)
            break;
        serviceStatus.dwControlsAccepted = 0;
        serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        serviceStatus.dwWin32ExitCode = 0;
        serviceStatus.dwCheckPoint = 4;
        SetEvent(serviceStopEvent);
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
    SetServiceStatus(statusHandle, &serviceStatus);
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
    (void)lpParam;
    bool isRunning = false;
    while ((WaitForSingleObject(serviceStopEvent, 0) != WAIT_OBJECT_0) && !isRunning)
    {
        RunDDMain();
        isRunning = true;
    }
    return ERROR_SUCCESS;
}
