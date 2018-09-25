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
#include <QLibraryInfo>
#include <QSysInfo>
#include <QVersionNumber>

//https://github.com/ThomasHuai/Wallpaper/blob/master/utils.cpp
HWND HWORKERW = nullptr;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    Q_UNUSED(lParam)
    HWND defview = FindWindowEx(hwnd, nullptr, TEXT("SHELLDLL_DefView"), nullptr);
    if (defview != nullptr)
    {
        HWORKERW = FindWindowEx(nullptr, hwnd, TEXT("WorkerW"), nullptr);
        if (HWORKERW != nullptr)
            return FALSE;
    }
    return TRUE;
}

HWND getProgman()
{
    return FindWindow(TEXT("Progman"), TEXT("Program Manager"));
}

HWND getWorkerW(bool legacyMode = false)
{
    HWND hwnd = getProgman();
    SendMessage(hwnd, 0x052c, 0, 0);
    EnumWindows(EnumWindowsProc, 0);
    ShowWindow(HWORKERW, legacyMode ? SW_HIDE : SW_SHOW);
    return legacyMode ? hwnd : HWORKERW;
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
    QTranslator ddTranslator;
    if (SettingsManager::getInstance()->getLocalize())
        if (ddTranslator.load(QLocale(), QStringLiteral("dd"), QStringLiteral("_"), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
            QApplication::installTranslator(&ddTranslator);
    int suffixIndex;
    QVersionNumber currentVersion = QVersionNumber::fromString(QSysInfo::kernelVersion(), &suffixIndex);
    QVersionNumber win7Version(6, 1, 7600);
    if (currentVersion < win7Version)
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("This application only supports Windows 7 and newer."));
        return 0;
    }
    HANDLE mutex = CreateMutex(nullptr, FALSE, TEXT("wangwenx190.DynamicDesktop.1000.AppMutex"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("There is another instance running. Please do not run twice."));
        return 0;
    }
    if (SettingsManager::getInstance()->getAutostart())
        SettingsManager::getInstance()->regAutostart();
    else
        SettingsManager::getInstance()->unregAutostart();
    QtAV::GLWidgetRenderer2 renderer;
    renderer.forcePreferredPixelFormat(true);
    renderer.setWindowIcon(QIcon(QStringLiteral(":/icon.ico")));
    renderer.setWindowTitle(QObject::tr("My wallpaper"));
    renderer.setAttribute(Qt::WA_NoSystemBackground);
    renderer.setWindowFlags(renderer.windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint | Qt::WindowDoesNotAcceptFocus);
    QRect screenGeometry = QApplication::desktop()->screenGeometry(&renderer);
    renderer.setGeometry(screenGeometry);
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
    QAction *playAction = trayMenu.addAction(QObject::tr("Play"));
    QObject::connect(playAction, &QAction::triggered,
        [=, &renderer, &player]
        {
            if (renderer.isHidden())
                renderer.show();
            player.play();
        });
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
    QAction *aboutAction = trayMenu.addAction(QObject::tr("About"));
    QObject::connect(aboutAction, &QAction::triggered,
        [=]
        {
            QMessageBox::about(nullptr, QStringLiteral("Dynamic Desktop"),
                               QObject::tr("Version: %0\nSource code: %1\nBuild time: %2 %3")
                                   .arg(QStringLiteral(DD_VERSION))
                                   .arg(QStringLiteral("https://github.com/wangwenx190/dynamic-desktop"))
                                   .arg(QStringLiteral(__DATE__))
                                   .arg(QStringLiteral(__TIME__)));
        });
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
    {
        if (renderer.isHidden())
            renderer.show();
        player.play(SettingsManager::getInstance()->getUrl());
    }
    else
        optionsAction->triggered();
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated,
        [=](QSystemTrayIcon::ActivationReason reason)
        {
            if (reason != QSystemTrayIcon::Context)
                optionsAction->triggered();
        });
    QObject::connect(&preferencesDialog, SIGNAL(about()), aboutAction, SIGNAL(triggered()));
    QObject::connect(&preferencesDialog, SIGNAL(play()), playAction, SIGNAL(triggered()));
    QObject::connect(&preferencesDialog, SIGNAL(pause()), &player, SLOT(pause()));
    QObject::connect(&preferencesDialog, &PreferencesDialog::urlChanged,
        [=, &player, &renderer](const QString &url)
        {
            if (!url.isEmpty())
            {
                if (renderer.isHidden())
                    renderer.show();
                player.play(url);
            }
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
            if (enabled)
                SettingsManager::getInstance()->regAutostart();
            else
                SettingsManager::getInstance()->unregAutostart();
        });
    HWND hworkerw = nullptr;
    auto hrenderer = reinterpret_cast<HWND>(renderer.winId());
    QVersionNumber win10Version(10, 0, 10240); // Windows 10 Version 1507
    // How to place our window under desktop icons:
    // Use "Program Manager" as our parent window in Win7/8/8.1.
    // Use "WorkerW" as our parent window in Win10.
    // Use "Program Manager" as our parent window in
    // Win10 is also OK, but our window will come
    // to front if we press "Win + Tab" and it will
    // also block our desktop icons, however using
    // "WorkerW" as our parent window will not result
    // in this problem, I don't know why. It's strange.
    hworkerw = getWorkerW(currentVersion < win10Version);
    if (hworkerw != nullptr)
        SetParent(hrenderer, hworkerw);
    int exec = QApplication::exec();
    ShowWindow(HWORKERW, SW_HIDE);
    CloseHandle(mutex);
    return exec;
}
