#include "forms/preferencesdialog.h"

#include <Windows.h>

#include <QApplication>
#include <QtAV>
#include <QtAVWidgets>
#include <QDesktopWidget>

//https://github.com/ThomasHuai/Wallpaper/blob/master/utils.cpp
static HWND HWORKERW = nullptr;

inline BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lparam)
{
    Q_UNUSED(lparam)
    HWND defview = FindWindowEx(hwnd, nullptr, TEXT("SHELLDLL_DefView"), nullptr);
    if (defview != nullptr)
    {
        HWORKERW = FindWindowEx(nullptr, hwnd, TEXT("WorkerW"), nullptr);
    }
    return TRUE;
}

HWND getWorkerW()
{
    int result;
    HWND hwnd = FindWindow(TEXT("Progman"), nullptr);
    // 使用 0x3e8 命令分割出两个 WorkerW
    SendMessageTimeout(hwnd, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, reinterpret_cast<PDWORD_PTR>(&result));
    // 遍历窗体获得窗口句柄
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(nullptr));
    return hwnd;
}

void showWorkerW()
{
    if (!HWORKERW)
        return;
    ShowWindow(HWORKERW, SW_SHOW);
}

void hideWorkerW()
{
    if (!HWORKERW)
        return;
    ShowWindow(HWORKERW, SW_HIDE);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    QtAV::GLWidgetRenderer2 renderer;
    renderer.setWindowFlags(renderer.windowFlags() | Qt::FramelessWindowHint);
    renderer.showFullScreen();
    QtAV::AVPlayer player;
    player.setRenderer(&renderer);
    //player.play(QStringLiteral("test.mkv"));
    PreferencesDialog preferencesDialog;
    preferencesDialog.show();
    HWND workerwHWND = getWorkerW();
    auto wallpaperHWND = reinterpret_cast<HWND>(renderer.winId());
    hideWorkerW();
    SetParent(wallpaperHWND, workerwHWND);
    QRect screenGeo = QApplication::desktop()->screenGeometry(&renderer);
    SetWindowPos(wallpaperHWND, HWND_TOP, 0, 0, screenGeo.width(), screenGeo.height(), SWP_NOACTIVATE);
    renderer.setGeometry(screenGeo);
    int exec = QApplication::exec();
    showWorkerW();
    return exec;
}
