#include "settingsmanager.h"
#include "skinsmanager.h"
#include "utils.h"
#include <Wallpaper>
#include "forms/preferencesdialog.h"
#include "forms/aboutdialog.h"
#include "forms/traymenu.h"
#include "playerwindow.h"
#include <QtSingleApplication>

#include <QMessageBox>
#include <QSysInfo>
#include <QVersionNumber>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSystemTrayIcon>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QtSingleApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QtSingleApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QtSingleApplication::setAttribute(Qt::AA_UseOpenGLES);
    QtSingleApplication app(QStringLiteral("wangwenx190.DynamicDesktop.Main.1000.AppMutex"), argc, argv);
    QtSingleApplication::setApplicationName(QStringLiteral("Dynamic Desktop"));
    QtSingleApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop"));
    QtSingleApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QtSingleApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
    if (app.sendMessage(QStringLiteral("show")))
        return 0;
#ifndef _DEBUG
    qInstallMessageHandler(Utils::fileLogger);
#endif
    Utils::installTranslation(SettingsManager::getInstance()->getLanguage(), QStringLiteral("dd"));
    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("A tool that make your desktop alive."));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption windowModeOption(QStringLiteral("window"),
                                        QtSingleApplication::translate("main", "Show a normal window instead of placing it under the desktop icons."));
    parser.addOption(windowModeOption);
    QCommandLineOption skinOption(QStringLiteral("skin"),
                                  QtSingleApplication::translate("main", "Set skin. The value is the file name of the skin file, excluding the file extension. If it's not under the \"skins\" folder, please give the absolute path of the file."),
                                  QtSingleApplication::translate("main", "Skin file name"));
    parser.addOption(skinOption);
    QCommandLineOption urlOption(QStringLiteral("url"),
                                 QtSingleApplication::translate("main", "Play the given url. It can be a local file or a valid web url. Default is empty."),
                                 QtSingleApplication::translate("main", "url"));
    parser.addOption(urlOption);
    QCommandLineOption imageQualityOption(QStringLiteral("quality"),
                                          QtSingleApplication::translate("main", "Set the quality of the output image. It can be default/best/fastest. Default is best. Case insensitive."),
                                          QtSingleApplication::translate("main", "Image quality"));
    parser.addOption(imageQualityOption);
    QCommandLineOption rendererOption(QStringLiteral("renderer"),
                                      QtSingleApplication::translate("main", "Set rendering engine. It can be opengl/gl/qt/gdi/d2d. Default is gl. Case insensitive."),
                                      QtSingleApplication::translate("main", "renderer"));
    parser.addOption(rendererOption);
    QCommandLineOption volumeOption(QStringLiteral("volume"),
                                    QtSingleApplication::translate("main", "Set volume. It must be a positive integer between 0 and 99. Default is 9."),
                                    QtSingleApplication::translate("main", "volume"));
    parser.addOption(volumeOption);
    parser.process(app);
    const bool windowMode = parser.isSet(windowModeOption);
    QString skinOptionValue = parser.value(skinOption);
    if (!skinOptionValue.isEmpty())
        if (skinOptionValue != SettingsManager::getInstance()->getSkin())
            SettingsManager::getInstance()->setSkin(skinOptionValue);
    QString urlOptionValue = parser.value(urlOption);
    if (!urlOptionValue.isEmpty())
        if (urlOptionValue != SettingsManager::getInstance()->getUrl())
            SettingsManager::getInstance()->setUrl(urlOptionValue);
    QString imageQualityOptionValue = parser.value(imageQualityOption).toLower();
    if (!imageQualityOptionValue.isEmpty())
        if (((imageQualityOptionValue == QStringLiteral("default")) ||
             (imageQualityOptionValue == QStringLiteral("best")) ||
             (imageQualityOptionValue == QStringLiteral("fastest"))) &&
                (imageQualityOptionValue != SettingsManager::getInstance()->getImageQuality()))
            SettingsManager::getInstance()->setImageQuality(imageQualityOptionValue);
    QString rendererOptionValue = parser.value(rendererOption).toLower();
    if (!rendererOptionValue.isEmpty())
        if ((rendererOptionValue == QStringLiteral("opengl")) &&
                (SettingsManager::getInstance()->getRenderer() != Utils::getVideoRendererId(Utils::VideoRendererId::OpenGLWidget)))
            SettingsManager::getInstance()->setRenderer(Utils::getVideoRendererId(Utils::VideoRendererId::OpenGLWidget));
        else if ((rendererOptionValue == QStringLiteral("gl")) &&
                 (SettingsManager::getInstance()->getRenderer() != Utils::getVideoRendererId(Utils::VideoRendererId::GLWidget2)))
            SettingsManager::getInstance()->setRenderer(Utils::getVideoRendererId(Utils::VideoRendererId::GLWidget2));
        else if ((rendererOptionValue == QStringLiteral("qt")) &&
                 (SettingsManager::getInstance()->getRenderer() != Utils::getVideoRendererId(Utils::VideoRendererId::Widget)))
            SettingsManager::getInstance()->setRenderer(Utils::getVideoRendererId(Utils::VideoRendererId::Widget));
        else if ((rendererOptionValue == QStringLiteral("gdi")) &&
                 (SettingsManager::getInstance()->getRenderer() != Utils::getVideoRendererId(Utils::VideoRendererId::GDI)))
            SettingsManager::getInstance()->setRenderer(Utils::getVideoRendererId(Utils::VideoRendererId::GDI));
        else if ((rendererOptionValue == QStringLiteral("d2d")) &&
                 (SettingsManager::getInstance()->getRenderer() != Utils::getVideoRendererId(Utils::VideoRendererId::Direct2D)))
            SettingsManager::getInstance()->setRenderer(Utils::getVideoRendererId(Utils::VideoRendererId::Direct2D));
    QString volumeOptionValue = parser.value(volumeOption);
    if (!volumeOptionValue.isEmpty())
    {
        int volumeOptionValueInt = volumeOptionValue.toInt();
        if (volumeOptionValueInt < 0)
            volumeOptionValueInt = 0;
        if (volumeOptionValueInt > 99)
            volumeOptionValueInt = 99;
        if (static_cast<quint32>(volumeOptionValueInt) != SettingsManager::getInstance()->getVolume())
            SettingsManager::getInstance()->setVolume(static_cast<quint32>(volumeOptionValueInt));
    }
    SkinsManager::getInstance()->setSkin(SettingsManager::getInstance()->getSkin());
    PlayerWindow playerWindow;
    PreferencesDialog preferencesDialog;
    AboutDialog aboutDialog;
    TrayMenu trayMenu;
    QSystemTrayIcon trayIcon;
    playerWindow.setWindowMode(windowMode);
    trayIcon.setIcon(QIcon(QStringLiteral(":/icons/color_palette.svg")));
    trayIcon.setContextMenu(&trayMenu);
    QObject::connect(&preferencesDialog, &PreferencesDialog::muteChanged, &trayMenu, &TrayMenu::setMute);
    QObject::connect(&preferencesDialog, &PreferencesDialog::about, &trayMenu, &TrayMenu::onAboutClicked);
    QObject::connect(&preferencesDialog, &PreferencesDialog::play, &playerWindow, &PlayerWindow::play);
    QObject::connect(&preferencesDialog, &PreferencesDialog::pause, &playerWindow, &PlayerWindow::pause);
    QObject::connect(&preferencesDialog, &PreferencesDialog::languageChanged, &aboutDialog, &AboutDialog::refreshTexts);
    QObject::connect(&preferencesDialog, &PreferencesDialog::languageChanged, &trayMenu, &TrayMenu::refreshTexts);
    QObject::connect(&preferencesDialog, &PreferencesDialog::urlChanged, &playerWindow, &PlayerWindow::setUrl);
    QObject::connect(&preferencesDialog, &PreferencesDialog::urlChanged, &trayIcon, &QSystemTrayIcon::setToolTip);
    QObject::connect(&preferencesDialog, &PreferencesDialog::audioFileChanged, &playerWindow, &PlayerWindow::setAudio);
    QObject::connect(&preferencesDialog, &PreferencesDialog::subtitleFileChanged, &playerWindow, &PlayerWindow::setSubtitle);
    QObject::connect(&preferencesDialog, &PreferencesDialog::volumeChanged, &playerWindow, &PlayerWindow::setVolume);
    QObject::connect(&preferencesDialog, &PreferencesDialog::seek, &playerWindow, &PlayerWindow::seek);
    QObject::connect(&preferencesDialog, &PreferencesDialog::videoTrackChanged, &playerWindow, &PlayerWindow::setVideoTrack);
    QObject::connect(&preferencesDialog, &PreferencesDialog::audioTrackChanged, &playerWindow, &PlayerWindow::setAudioTrack);
    QObject::connect(&preferencesDialog, &PreferencesDialog::subtitleTrackChanged, &playerWindow, &PlayerWindow::setSubtitleTrack);
    QObject::connect(&preferencesDialog, &PreferencesDialog::rendererChanged, &playerWindow, &PlayerWindow::setRenderer);
    QObject::connect(&preferencesDialog, &PreferencesDialog::imageQualityChanged, &playerWindow, &PlayerWindow::setImageQuality);
    QObject::connect(&preferencesDialog, &PreferencesDialog::charsetChanged, &playerWindow, &PlayerWindow::setCharset);
    QObject::connect(&preferencesDialog, &PreferencesDialog::subtitleAutoLoadChanged, &playerWindow, &PlayerWindow::setSubtitleAutoLoad);
    QObject::connect(&preferencesDialog, &PreferencesDialog::subtitleEnableChanged, &playerWindow, &PlayerWindow::setSubtitleEnabled);
    QObject::connect(&preferencesDialog, &PreferencesDialog::imageRatioChanged, &playerWindow, &PlayerWindow::setImageRatio);
    QObject::connect(&playerWindow, &PlayerWindow::playStateChanged, &preferencesDialog, &PreferencesDialog::setPlaying);
    QObject::connect(&playerWindow, &PlayerWindow::playStateChanged, &trayMenu, &TrayMenu::setPlaying);
    QObject::connect(&playerWindow, &PlayerWindow::volumeToolTipChanged, &preferencesDialog, &PreferencesDialog::setVolumeToolTip);
    QObject::connect(&playerWindow, &PlayerWindow::mediaPositionChanged, &preferencesDialog, &PreferencesDialog::setMediaSliderPosition);
    QObject::connect(&playerWindow, &PlayerWindow::videoPositionTextChanged, &preferencesDialog, &PreferencesDialog::setVideoPositionText);
    QObject::connect(&playerWindow, &PlayerWindow::audioAreaEnableChanged, &preferencesDialog, &PreferencesDialog::setAudioAreaEnabled);
    QObject::connect(&playerWindow, &PlayerWindow::clearAllTracks, &preferencesDialog, &PreferencesDialog::clearAllTracks);
    QObject::connect(&playerWindow, &PlayerWindow::mediaSliderUnitChanged, &preferencesDialog, &PreferencesDialog::setMediaSliderUnit);
    QObject::connect(&playerWindow, &PlayerWindow::mediaSliderRangeChanged, &preferencesDialog, &PreferencesDialog::setMediaSliderRange);
    QObject::connect(&playerWindow, &PlayerWindow::seekAreaEnableChanged, &preferencesDialog, &PreferencesDialog::setSeekAreaEnabled);
    QObject::connect(&playerWindow, &PlayerWindow::videoTracksChanged, &preferencesDialog, &PreferencesDialog::setVideoTracks);
    QObject::connect(&playerWindow, &PlayerWindow::audioTracksChanged, &preferencesDialog, &PreferencesDialog::setAudioTracks);
    QObject::connect(&playerWindow, &PlayerWindow::subtitleTracksChanged, &preferencesDialog, &PreferencesDialog::setSubtitleTracks);
    QObject::connect(&playerWindow, &PlayerWindow::videoDurationTextChanged, &preferencesDialog, &PreferencesDialog::setVideoDurationText);
    QObject::connect(&trayMenu, &TrayMenu::onOptionsClicked, [=, &preferencesDialog]
    {
        if (preferencesDialog.isHidden())
        {
            Utils::moveToCenter(&preferencesDialog);
            preferencesDialog.show();
        }
        if (!preferencesDialog.isActiveWindow())
            preferencesDialog.setWindowState(preferencesDialog.windowState() & ~Qt::WindowMinimized);
        if (!preferencesDialog.isActiveWindow())
        {
            preferencesDialog.raise();
            preferencesDialog.activateWindow();
        }
    });
    QObject::connect(&trayMenu, &TrayMenu::onPlayClicked, &preferencesDialog, &PreferencesDialog::togglePlayPause);
    trayMenu.setMute(SettingsManager::getInstance()->getMute());
    QObject::connect(&trayMenu, &TrayMenu::onMuteClicked, [=, &preferencesDialog]
    {
        preferencesDialog.setMute(!SettingsManager::getInstance()->getMute());
    });
    QObject::connect(&trayMenu, &TrayMenu::onAboutClicked, [=, &aboutDialog]
    {
        if (aboutDialog.isHidden())
        {
            Utils::moveToCenter(&aboutDialog);
            aboutDialog.show();
        }
        if (!aboutDialog.isActiveWindow())
            aboutDialog.setWindowState(aboutDialog.windowState() & ~Qt::WindowMinimized);
        if (!aboutDialog.isActiveWindow())
        {
            aboutDialog.raise();
            aboutDialog.activateWindow();
        }
    });
    QObject::connect(&trayMenu, &TrayMenu::onExitClicked, &app, &QtSingleApplication::quit);
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, [=, &trayMenu](QSystemTrayIcon::ActivationReason reason)
    {
        if (reason != QSystemTrayIcon::Context)
            emit trayMenu.onOptionsClicked();
    });
    QObject::connect(qApp, &QtSingleApplication::aboutToQuit, [=]
    {
        Utils::installTranslation(QStringLiteral("en"), QStringLiteral("dd"));
        Wallpaper::hideWallpaper();
    });
    QObject::connect(&app, &QtSingleApplication::messageReceived, [=, &trayMenu](const QString &message)
    {
        Q_UNUSED(message)
        emit trayMenu.onOptionsClicked();
    });
    trayIcon.show();
    const Qt::WindowFlags windowFlags = Qt::FramelessWindowHint;
    const QRect screenGeometry = QtSingleApplication::desktop()->screenGeometry(&playerWindow);
    if (!windowMode)
    {
        playerWindow.setWindowFlags(windowFlags);
        // Why is Direct2D image too large?
        playerWindow.setGeometry(screenGeometry);
        int suffixIndex = 0;
        QVersionNumber currentVersion = QVersionNumber::fromString(QSysInfo::kernelVersion(), &suffixIndex);
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
        Wallpaper::setLegacyMode(currentVersion < win10Version);
        Wallpaper::setWallpaper(reinterpret_cast<HWND>(playerWindow.winId()));
    }
    else
    {
        playerWindow.resize(1280, 720);
        Utils::moveToCenter(&playerWindow);
        if (playerWindow.isHidden())
            playerWindow.show();
    }
    if (SettingsManager::getInstance()->getUrl().isEmpty())
    {
        trayIcon.setToolTip(QStringLiteral("Dynamic Desktop"));
        Utils::moveToCenter(&preferencesDialog);
        if (preferencesDialog.isHidden())
            preferencesDialog.show();
    }
    else
    {
        const QString url = SettingsManager::getInstance()->getUrl();
        trayIcon.setToolTip(url);
        if (playerWindow.isHidden())
            playerWindow.show();
        playerWindow.setUrl(url);
    }
    return QtSingleApplication::exec();
}
