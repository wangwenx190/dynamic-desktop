#ifdef UNICODE
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"wmainCRTStartup\"")
#else
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
#endif

#include <Windows.h>
#include <shellapi.h>
#include <tchar.h>

using externalMain = int (*)(int, char **);

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
    int exec = -1;
    bool failed = true;
    HINSTANCE module = nullptr;
    switch (startType)
    {
    case StartType::player:
        module = LoadLibrary(_T("player1.dll"));
        if (module != nullptr)
        {
            auto moduleMain = (externalMain)GetProcAddress(module, "playerMain");
            if (moduleMain != nullptr)
            {
                failed = false;
                exec = moduleMain(argcA, argvA);
            }
            FreeLibrary(module);
        }
        break;
    case StartType::service:
        module = LoadLibrary(_T("service1.dll"));
        if (module != nullptr)
        {
            auto moduleMain = (externalMain)GetProcAddress(module, "serviceMain");
            if (moduleMain != nullptr)
            {
                failed = false;
                exec = moduleMain(argcA, argvA);
            }
            FreeLibrary(module);
        }
        break;
    case StartType::updater:
        module = LoadLibrary(_T("updater1.dll"));
        if (module != nullptr)
        {
            auto moduleMain = (externalMain)GetProcAddress(module, "updaterMain");
            if (moduleMain != nullptr)
            {
                failed = false;
                exec = moduleMain(argcA, argvA);
            }
            FreeLibrary(module);
        }
        break;
    case StartType::controller:
    default:
        module = LoadLibrary(_T("controller1.dll"));
        if (module != nullptr)
        {
            auto moduleMain = (externalMain)GetProcAddress(module, "controllerMain");
            if (moduleMain != nullptr)
            {
                failed = false;
                exec = moduleMain(argcA, argvA);
            }
            FreeLibrary(module);
        }
        break;
    }
    for (int i = 0; i != argcA && argvA[i]; ++i)
        delete [] argvA[i];
    delete [] argvA;
    if (failed)
        MessageBox(nullptr, _T("Failed to load main module!\nContact the developers for technical support."), _T("Error"), MB_OK | MB_ICONERROR);
    return exec;
}
