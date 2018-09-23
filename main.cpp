#include "forms/preferencesdialog.h"
#include "settingsmanager.h"

#include <Windows.h>

#include <QApplication>
#include <QtAV>
#include <QtAVWidgets>
#include <QDesktopWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QMessageBox>
#include <QTranslator>
#include <QLocale>

//https://github.com/ThomasHuai/Wallpaper/blob/master/utils.cpp
HWND HWORKERW = nullptr;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    Q_UNUSED(lParam)
    HWND HDEFVIEW = FindWindowEx(hwnd, nullptr, TEXT("SHELLDLL_DefView"), nullptr);
    if (HDEFVIEW != nullptr)
    {
        HWORKERW = FindWindowEx(nullptr, hwnd, TEXT("WorkerW"), nullptr);
        if (HWORKERW != nullptr)
            return FALSE;
    }
    return TRUE;
}

HWND getProgman()
{
    int result;
    HWND hwnd = FindWindow(TEXT("Progman"), nullptr);
    // 使用 0x3e8 命令分割出两个 WorkerW
    SendMessageTimeout(hwnd, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, reinterpret_cast<PDWORD_PTR>(&result));
    // 遍历窗体获得窗口句柄
    EnumWindows(EnumWindowsProc, 0);
    ShowWindow(HWORKERW, SW_HIDE);
    return hwnd;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setApplicationName(QStringLiteral("Dynamic Desktop"));
    QApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop"));
    QCoreApplication::setApplicationVersion(QStringLiteral(DD_VERSION));
    QCoreApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
    QApplication app(argc, argv);
    AllowSetForegroundWindow(static_cast<DWORD>(QApplication::applicationPid()));
    QTranslator translator;
    if (translator.load(QLocale(), QStringLiteral("dd"), QStringLiteral("_"), QStringLiteral(":/i18n")))
        QApplication::installTranslator(&translator);
    HANDLE mutex = CreateMutex(nullptr, FALSE, TEXT("wangwenx190.DynamicDesktop.1000.AppMutex"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("There is another instance running."));
        CloseHandle(mutex);
        return 0;
    }
    if (SettingsManager::getInstance()->getAutostart() && !SettingsManager::getInstance()->isRegAutostart())
        SettingsManager::getInstance()->regAutostart();
    else if (!SettingsManager::getInstance()->getAutostart() && SettingsManager::getInstance()->isRegAutostart())
        SettingsManager::getInstance()->unregAutostart();
    QtAV::GLWidgetRenderer2 renderer;
    renderer.forcePreferredPixelFormat(true);
    renderer.setWindowIcon(QIcon(QStringLiteral(":/icon.ico")));
    renderer.setWindowTitle(QObject::tr("My wallpaper"));
    QRect screenGeometry = QApplication::desktop()->screenGeometry(&renderer);
    renderer.setAttribute(Qt::WA_NoSystemBackground);
    renderer.setWindowFlags(renderer.windowFlags() | Qt::FramelessWindowHint);
    renderer.setGeometry(screenGeometry.left(), screenGeometry.bottom(), 5, 5);
    renderer.show();
    QtAV::AVPlayer player;
    player.setRenderer(&renderer);
    if (SettingsManager::getInstance()->getHwdec())
    {
        QStringList decoders = SettingsManager::getInstance()->getDecoders();
        if (!decoders.contains(QStringLiteral("FFmpeg")))
            decoders << QStringLiteral("FFmpeg");
        player.setVideoDecoderPriority(decoders);
        if (decoders.contains(QStringLiteral("CUDA")))
        {
            QVariantHash cuda_opt;
            cuda_opt[QStringLiteral("surfaces")] = 0;
            cuda_opt[QStringLiteral("copyMode")] = QStringLiteral("ZeroCopy");
            QVariantHash opt;
            opt[QStringLiteral("CUDA")] = cuda_opt;
            player.setOptionsForVideoCodec(opt);
        }
    }
    player.setRepeat(-1);
    QObject::connect(&player, SIGNAL(stopped()), &player, SLOT(play()));
    PreferencesDialog preferencesDialog;
    preferencesDialog.setWindowIcon(QIcon(QStringLiteral(":/icon.ico")));
    QMenu trayMenu;
    QAction *optionsAction = trayMenu.addAction(QObject::tr("Preferences"));
    QObject::connect(optionsAction, &QAction::triggered,
        [=, &preferencesDialog]
        {
            if (preferencesDialog.isHidden())
                preferencesDialog.show();
            if (!preferencesDialog.isActiveWindow())
                preferencesDialog.setWindowState(preferencesDialog.windowState() & ~Qt::WindowMinimized);
            if (!preferencesDialog.isActiveWindow())
            {
                preferencesDialog.raise();
                preferencesDialog.activateWindow();
            }
        });
    trayMenu.addSeparator();
    trayMenu.addAction(QObject::tr("Play"), &player, SLOT(play()));
    trayMenu.addAction(QObject::tr("Pause"), &player, SLOT(pause()));
    QAction *muteAction = trayMenu.addAction(QObject::tr("Mute"));
    muteAction->setCheckable(true);
    QObject::connect(muteAction, &QAction::triggered,
        [=, &player, &preferencesDialog](bool checked)
        {
            if (player.audio())
            {
                muteAction->setChecked(checked);
                player.audio()->setMute(checked);
                SettingsManager::getInstance()->setMute(checked);
                preferencesDialog.refreshUi();
            }
        });
    trayMenu.addSeparator();
    trayMenu.addAction(QObject::tr("Exit"), qApp, &QApplication::closeAllWindows);
    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(QStringLiteral(":/icon.ico")));
    trayIcon.setToolTip(QStringLiteral("Dynamic Desktop"));
    trayIcon.setContextMenu(&trayMenu);
    trayIcon.show();
    if (player.audio())
    {
        player.audio()->setVolume(static_cast<qreal>(SettingsManager::getInstance()->getVolume() / 10.0));
        player.audio()->setMute(SettingsManager::getInstance()->getMute());
        muteAction->setCheckable(true);
        muteAction->setChecked(SettingsManager::getInstance()->getMute());
    }
    else
    {
        muteAction->setCheckable(false);
        muteAction->setEnabled(false);
        preferencesDialog.setAudioAreaEnabled(false);
    }
    if (!SettingsManager::getInstance()->getUrl().isEmpty())
        player.play(SettingsManager::getInstance()->getUrl());
    else
        optionsAction->triggered();
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated,
        [=](QSystemTrayIcon::ActivationReason reason)
        {
            if (reason != QSystemTrayIcon::Context)
                optionsAction->triggered();
        });
    QObject::connect(&preferencesDialog, SIGNAL(play()), &player, SLOT(play()));
    QObject::connect(&preferencesDialog, SIGNAL(pause()), &player, SLOT(pause()));
    QObject::connect(&preferencesDialog, &PreferencesDialog::urlChanged,
        [=, &player](const QString &url)
        {
            if (!url.isEmpty())
                player.play(url);
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::volumeChanged,
        [=, &player](unsigned int volume)
        {
            if (player.audio())
                player.audio()->setVolume(static_cast<qreal>(volume / 10.0));
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::muteChanged,
        [=, &player](bool mute)
        {
            if (player.audio())
                muteAction->triggered(mute);
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::autostartChanged,
        [=](bool enabled)
        {
            if (enabled && !SettingsManager::getInstance()->isRegAutostart())
                SettingsManager::getInstance()->regAutostart();
            else if (!enabled && SettingsManager::getInstance()->isRegAutostart())
                SettingsManager::getInstance()->unregAutostart();
        });
    HWND progman = getProgman();
    auto wallpaper = reinterpret_cast<HWND>(renderer.winId());
    SetParent(wallpaper, progman);
    SetWindowPos(wallpaper, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
    renderer.setGeometry(screenGeometry);
    int exec = QApplication::exec();
    CloseHandle(mutex);    
    return exec;
}
