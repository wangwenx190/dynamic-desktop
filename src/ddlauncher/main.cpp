#ifdef UNICODE
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"wmainCRTStartup\"")
#else
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
#endif

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <shellscalingapi.h>

using MainEntryFunc = int (*)(int, char **);

#ifdef UNICODE
// Convert a wchar_t to char string, equivalent to QString::toLocal8Bit()
// when passed CP_ACP.
static inline char *wideToMulti(int codePage, const wchar_t *aw)
{
    const int required = WideCharToMultiByte(codePage, 0, aw, -1, nullptr, 0, nullptr, nullptr);
    char *result = new char[required];
    WideCharToMultiByte(codePage, 0, aw, -1, result, required, nullptr, nullptr);
    return result;
}
#endif

int _tmain(int argc, TCHAR *argv[])
{
    HINSTANCE mainModuleLib = nullptr;
    MainEntryFunc mainEntryFunc = nullptr;
    BOOL initSucceeded = FALSE;
    int exec = -1;
#ifdef UNICODE
    (void)argc;
    (void)argv;
    int argcW;
    wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argcW);
    if (argvW == nullptr)
        return exec;
    char **argvA = new char *[argcW + 1];
    for (int i = 0; i != argcW; ++i)
        argvA[i] = wideToMulti(CP_ACP, argvW[i]);
    argvA[argcW] = nullptr;
    LocalFree(argvW);
#endif
#ifdef _DEBUG
    mainModuleLib = LoadLibrary(TEXT("DDMaind"));
#else
    mainModuleLib = LoadLibrary(TEXT("DDMain"));
#endif
    if (mainModuleLib != nullptr)
    {
        mainEntryFunc = reinterpret_cast<MainEntryFunc>(GetProcAddress(mainModuleLib, "ddmain"));
        if (mainEntryFunc != nullptr)
        {
            initSucceeded = TRUE;
#ifdef UNICODE
            exec = mainEntryFunc(argcW, argvA);
#else
            exec = mainEntryFunc(argc, argv);
#endif
        }
        FreeLibrary(mainModuleLib);
    }
#ifdef UNICODE
    for (int i = 0; ((i != argcW) && (argvA[i] != nullptr)); ++i)
        delete [] argvA[i];
    delete [] argvA;
#endif
    if (initSucceeded != TRUE)
    {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        MessageBox(nullptr, TEXT("Failed to load main module or resolve entry function. Application will not start.\nReinstalling this application may fix this problem.\nContact the developers for more information.\nSorry for the inconvenience."), TEXT("ERROR"), MB_ICONERROR | MB_OK);
    }
    return exec;
}
