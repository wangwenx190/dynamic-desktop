// Hide the console window although it's a console program
#ifdef UNICODE
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"wmainCRTStartup\"")
#else
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
#endif

#include <windows.h>
#include <tchar.h>

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

bool enableHiDPISupport()
{
    // SetProcessDpiAwarenessContext: https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setprocessdpiawarenesscontext
    struct DD_DPI_AWARENESS_CONTEXT__ {
        int unused;
    };
    using DD_DPI_AWARENESS_CONTEXT = struct DD_DPI_AWARENESS_CONTEXT__ *;
    using pfnSetProcessDpiAwarenessContext = BOOL (*)(DD_DPI_AWARENESS_CONTEXT);
    HMODULE user32Lib = GetModuleHandle(TEXT("User32"));
    if (user32Lib != nullptr)
    {
        auto setProcessDpiAwarenessContext = reinterpret_cast<pfnSetProcessDpiAwarenessContext>(GetProcAddress(user32Lib, "SetProcessDpiAwarenessContext"));
        if (setProcessDpiAwarenessContext != nullptr)
        {
            // #define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE         ((DPI_AWARENESS_CONTEXT)-2)
            // #define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE    ((DPI_AWARENESS_CONTEXT)-3)
            // #define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
            BOOL result = setProcessDpiAwarenessContext((DD_DPI_AWARENESS_CONTEXT)-4);
            if (result != TRUE)
                result = setProcessDpiAwarenessContext((DD_DPI_AWARENESS_CONTEXT)-3);
            if (result != TRUE)
                result = setProcessDpiAwarenessContext((DD_DPI_AWARENESS_CONTEXT)-2);
            return (result == TRUE);
        }
    }
    // SetProcessDpiAwareness: https://docs.microsoft.com/en-us/windows/desktop/api/shellscalingapi/nf-shellscalingapi-setprocessdpiawareness
    using DD_PROCESS_DPI_AWARENESS = enum _DD_PROCESS_DPI_AWARENESS {
        PROCESS_DPI_UNAWARE = 0,
        PROCESS_SYSTEM_DPI_AWARE = 1,
        PROCESS_PER_MONITOR_DPI_AWARE = 2
    };
    using pfnSetProcessDpiAwareness = HRESULT (*)(DD_PROCESS_DPI_AWARENESS);
    HMODULE shcoreLib = GetModuleHandle(TEXT("Shcore"));
    if (shcoreLib != nullptr)
    {
        auto setProcessDpiAwareness = reinterpret_cast<pfnSetProcessDpiAwareness>(GetProcAddress(shcoreLib, "SetProcessDpiAwareness"));
        if (setProcessDpiAwareness != nullptr)
            return SUCCEEDED(setProcessDpiAwareness(DD_PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE));
    }
    // SetProcessDPIAware: https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setprocessdpiaware
    using pfnSetProcessDPIAware = BOOL (*)();
    if (user32Lib != nullptr)
    {
        auto setProcessDPIAware = reinterpret_cast<pfnSetProcessDPIAware>(GetProcAddress(user32Lib, "SetProcessDPIAware"));
        if (setProcessDPIAware != nullptr)
            return (setProcessDPIAware() == TRUE);
    }
    return false;
}

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
    enableHiDPISupport();
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
