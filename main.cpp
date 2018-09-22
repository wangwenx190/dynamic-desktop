#include "forms/preferencesdialog.h"

#include <Windows.h>

#include <QApplication>
#include <QtAV>
#include <QtAVWidgets>
#include <QDesktopWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QIcon>

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
    QCoreApplication::setApplicationName(QStringLiteral("Dynamic Desktop"));
    QCoreApplication::setApplicationVersion(QStringLiteral(DD_VERSION));
    QApplication app(argc, argv);    
    QtAV::GLWidgetRenderer2 renderer;
    renderer.setWindowIcon(QIcon(QStringLiteral(":/icon.ico")));
    renderer.setWindowTitle(QStringLiteral("Dynamic Desktop"));
    QRect screenGeometry = QApplication::desktop()->screenGeometry(&renderer);
    renderer.setAttribute(Qt::WA_NoSystemBackground);
    renderer.setWindowFlags(renderer.windowFlags() | Qt::FramelessWindowHint);
    renderer.setGeometry(screenGeometry.left(), screenGeometry.bottom(), 5, 5);
    renderer.show();
    QtAV::AVPlayer player;
    player.setRenderer(&renderer);
    //player.play(QStringLiteral("test.mkv"));
    PreferencesDialog preferencesDialog;
    preferencesDialog.setWindowIcon(QIcon(QStringLiteral(":/icon.ico")));
    QMenu trayMenu;
    trayMenu.addAction(QStringLiteral("Preferences"), &preferencesDialog, &QWidget::show);
    trayMenu.addAction(QStringLiteral("Exit"), qApp, &QApplication::closeAllWindows);
    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(QStringLiteral(":/icon.ico")));
    trayIcon.setToolTip(QStringLiteral("Dynamic Desktop"));
    trayIcon.setContextMenu(&trayMenu);
    trayIcon.show();
    //preferencesDialog.show();
    HWND workerwHWND = getWorkerW();
    hideWorkerW();
    auto wallpaperHWND = reinterpret_cast<HWND>(renderer.winId());
    SetParent(wallpaperHWND, workerwHWND);    
    renderer.setGeometry(screenGeometry);
    int exec = QApplication::exec();
    showWorkerW();
    return exec;
}
