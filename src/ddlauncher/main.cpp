// Hide the console window although it's a console program
#ifdef UNICODE
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"wmainCRTStartup\"")
#else
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
#endif

#include <windows.h>
#include <tchar.h>

#include <Win32Utils>

#ifdef UNICODE
// Copied from: http://code.qt.io/cgit/qt/qtbase.git/tree/src/winmain/qtmain_win.cpp?h=dev
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
    bool initSucceeded = false;
    if (!Win32Utils::setProcessDpiAwarenessContext())
        if (!Win32Utils::setProcessDpiAwareness())
            Win32Utils::setProcessDPIAware();
#ifdef _DEBUG
    HINSTANCE ddmainLib = LoadLibrary(TEXT("DDMaind"));
#else
    HINSTANCE ddmainLib = LoadLibrary(TEXT("DDMain"));
#endif
    if (ddmainLib != nullptr)
    {
        using pfnddmain = int (*)(int, char **);
        auto ddmain = reinterpret_cast<pfnddmain>(GetProcAddress(ddmainLib, "ddmain"));
        if (ddmain != nullptr)
        {
            initSucceeded = true;
#ifdef UNICODE
            exec = ddmain(argcW, argvA);
#else
            exec = ddmain(argc, argv);
#endif
        }
        FreeLibrary(ddmainLib);
    }
#ifdef UNICODE
    for (int i = 0; ((i != argcW) && (argvA[i] != nullptr)); ++i)
        delete [] argvA[i];
    delete [] argvA;
#endif
    if (!initSucceeded)
        MessageBox(nullptr, TEXT("Failed to load main module or resolve entry function. Application will not start.\nReinstalling the application may fix this problem.\nContact the developers for more information.\nSorry for the inconvenience."), TEXT("ERROR"), MB_ICONERROR | MB_OK);
    return exec;
}
