#ifdef UNICODE
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"wmainCRTStartup\"")
#else
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
#endif

#include <Windows.h>
#include <shellapi.h>
#include <tchar.h>

using ExternalMain = int (*)(int, char **);

enum StartType
{
    controller,
    player,
    service,
    updater
};

static inline char *wideToMulti(int codePage, const wchar_t *aw)
{
    const int required = WideCharToMultiByte(codePage, 0, aw, -1, nullptr, 0, nullptr, nullptr);
    auto *result = new char[required];
    WideCharToMultiByte(codePage, 0, aw, -1, result, required, nullptr, nullptr);
    return result;
}

int _tmain(int argc, TCHAR *argv[])
{
    (void)argc;
    (void)argv;
    StartType startType = StartType::controller;
    int argcW;
    wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argcW);
    if (!argvW)
        return -1;
    auto argvA = new char *[argcW + 1];
    int argcA = 0;
    for (int i = 0; i != argcW; ++i)
    {
        if (_tcscmp(argvW[i], _T("--controller")) == 0)
            continue;
        else if (_tcscmp(argvW[i], _T("--player")) == 0)
        {
            startType = StartType::player;
            continue;
        }
        else if (_tcscmp(argvW[i], _T("--service")) == 0)
        {
            startType = StartType::service;
            continue;
        }
        else if (_tcscmp(argvW[i], _T("--updater")) == 0)
        {
            startType = StartType::updater;
            continue;
        }
        argvA[argcA] = wideToMulti(CP_ACP, argvW[i]);
        ++argcA;
    }
    for (int i = argcA + 1; i <= argcW; ++i)
        argvA[i] = nullptr;
    LocalFree(argvW);
#ifdef _DEBUG
    HINSTANCE module = LoadLibrary(_T("cored1"));
#else
    HINSTANCE module = LoadLibrary(_T("core1"));
#endif
    ExternalMain moduleMain = nullptr;
    if (module != nullptr)
        switch (startType)
        {
        case StartType::player:
            moduleMain = (ExternalMain)GetProcAddress(module, "playerMain");
            break;
        case StartType::service:
            moduleMain = (ExternalMain)GetProcAddress(module, "serviceMain");
            break;
        case StartType::updater:
            moduleMain = (ExternalMain)GetProcAddress(module, "updaterMain");
            break;
        case StartType::controller:
        default:
            moduleMain = (ExternalMain)GetProcAddress(module, "controllerMain");
            break;
        }
    int exec = -1;
    if (moduleMain != nullptr)
        exec = moduleMain(argcA, argvA);
    if (module != nullptr)
        FreeLibrary(module);
    for (int i = 0; i != argcA && argvA[i]; ++i)
        delete [] argvA[i];
    delete [] argvA;
    if (module == nullptr || moduleMain == nullptr)
        MessageBox(nullptr, _T("Failed to load main module, application aborting.\nReinstalling this application may fix this problem."), _T("Error"), MB_OK | MB_ICONERROR);
    return exec;
}
