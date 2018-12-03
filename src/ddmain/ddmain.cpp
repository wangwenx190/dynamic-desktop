#include "ddmain.h"
#include "settingsmanager.h"
#ifndef DD_NO_CSS
#include "skinsmanager.h"
#endif
#include "utils.h"
#include <Wallpaper>
#include "forms/preferencesdialog.h"
#include "forms/aboutdialog.h"
#ifndef DD_NO_MENU
#include "forms/traymenu.h"
#endif
#include "playerwindow.h"
#include <QtSingleApplication>
#include "forms/playlistdialog.h"

#include <QMessageBox>
#include <QOperatingSystemVersion>
#ifndef DD_NO_COMMANDLINE_PARSER
#include <QCommandLineParser>
#include <QCommandLineOption>
#endif
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#ifndef DD_NO_TRANSLATIONS
#include <QTranslator>
#include <QLocale>

void installTranslation(const QString &lang, const QTranslator &translator)
{
    auto ts = const_cast<QTranslator *>(&translator);
    QtSingleApplication::removeTranslator(ts);
    if (lang.startsWith(QStringLiteral("en"), Qt::CaseInsensitive))
        return;
    const QString qmDir = QStringLiteral(":/i18n");
    if (lang.toLower() == QLatin1String("auto"))
    {
        if (ts->load(QLocale(), QStringLiteral("dd"), QStringLiteral("_"), qmDir))
            QtSingleApplication::installTranslator(ts);
    }
    else
    {
        if (lang.contains(QLatin1Char('/')) || lang.contains(QLatin1Char('\\')))
        {
            if (ts->load(lang))
                QtSingleApplication::installTranslator(ts);
        }
        else
        {
            const QString fileName = QStringLiteral("dd_%0").arg(lang);
            if (ts->load(fileName, qmDir))
                QtSingleApplication::installTranslator(ts);
        }
    }
}
#endif

int ddmain(int argc, char *argv[])
{
    QtSingleApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QtSingleApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    const QString openglType = SettingsManager::getInstance()->getOpenGLType();
    if (openglType == QLatin1String("gl"))
        QtSingleApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    else if (openglType == QLatin1String("egl"))
        QtSingleApplication::setAttribute(Qt::AA_UseOpenGLES);
    else if (openglType == QLatin1String("sw"))
        QtSingleApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QtSingleApplication app(QStringLiteral("wangwenx190.DynamicDesktop.Main.1000.AppMutex"), argc, argv);
    QtSingleApplication::setApplicationName(QStringLiteral("Dynamic Desktop"));
    QtSingleApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop"));
    QtSingleApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QtSingleApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
    if (app.sendMessage(QStringLiteral("show")))
        return 0;
#ifndef DD_NO_TRANSLATIONS
    QTranslator ddTranslator;
    installTranslation(SettingsManager::getInstance()->getLanguage(), ddTranslator);
#endif
    bool windowMode = false;
#ifndef DD_NO_COMMANDLINE_PARSER
    QCommandLineParser parser;
    parser.setApplicationDescription(DD_OBJ_TR("A tool that make your desktop alive."));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption windowModeOption(QStringLiteral("window"),
                                        DD_APP_TR("main", "Show a normal window instead of placing it under the desktop icons."));
    parser.addOption(windowModeOption);
#ifndef DD_NO_CSS
    QCommandLineOption skinOption(QStringLiteral("skin"),
                                  DD_APP_TR("main", "Set skin. The value is the file name of the skin file, excluding the file extension. If it's not under the \"skins\" folder, please give the absolute path of the file."),
                                  DD_APP_TR("main", "Skin file name"));
    parser.addOption(skinOption);
#endif
    QCommandLineOption urlOption(QStringLiteral("url"),
                                 DD_APP_TR("main", "Play the given url. It can be a local file or a valid web url. Default is empty."),
                                 DD_APP_TR("main", "url"));
    parser.addOption(urlOption);
    QCommandLineOption imageQualityOption(QStringLiteral("quality"),
                                          DD_APP_TR("main", "Set the quality of the output image. It can be default/best/fastest. Default is best. Case insensitive."),
                                          DD_APP_TR("main", "Image quality"));
    parser.addOption(imageQualityOption);
    QCommandLineOption rendererOption(QStringLiteral("renderer"),
                                      DD_APP_TR("main", "Set rendering engine. It can be opengl/gl/qt/gdi/d2d. Default is gl. Case insensitive."),
                                      DD_APP_TR("main", "renderer"));
    parser.addOption(rendererOption);
    QCommandLineOption volumeOption(QStringLiteral("volume"),
                                    DD_APP_TR("main", "Set volume. It must be a positive integer between 0 and 99. Default is 9."),
                                    DD_APP_TR("main", "volume"));
    parser.addOption(volumeOption);
    parser.process(app);
    windowMode = parser.isSet(windowModeOption);
#ifndef DD_NO_CSS
    QString skinOptionValue = parser.value(skinOption);
    if (!skinOptionValue.isEmpty())
        if (skinOptionValue != SettingsManager::getInstance()->getSkin())
            SettingsManager::getInstance()->setSkin(skinOptionValue);
#endif
    QString urlOptionValue = parser.value(urlOption);
    if (!urlOptionValue.isEmpty())
        if (urlOptionValue != SettingsManager::getInstance()->getLastFile())
            SettingsManager::getInstance()->setLastFile(urlOptionValue);
    QString imageQualityOptionValue = parser.value(imageQualityOption).toLower();
    if (!imageQualityOptionValue.isEmpty())
        if (((imageQualityOptionValue == QLatin1String("default")) ||
             (imageQualityOptionValue == QLatin1String("best")) ||
             (imageQualityOptionValue == QLatin1String("fastest"))) &&
                (imageQualityOptionValue != SettingsManager::getInstance()->getImageQuality()))
            SettingsManager::getInstance()->setImageQuality(imageQualityOptionValue);
    QString rendererOptionValue = parser.value(rendererOption).toLower();
    if (!rendererOptionValue.isEmpty())
        if ((rendererOptionValue == QLatin1String("opengl")) &&
                (SettingsManager::getInstance()->getRenderer() != Utils::getVideoRendererId(Utils::VideoRendererId::OpenGLWidget)))
            SettingsManager::getInstance()->setRenderer(Utils::getVideoRendererId(Utils::VideoRendererId::OpenGLWidget));
        else if ((rendererOptionValue == QLatin1String("gl")) &&
                 (SettingsManager::getInstance()->getRenderer() != Utils::getVideoRendererId(Utils::VideoRendererId::GLWidget2)))
            SettingsManager::getInstance()->setRenderer(Utils::getVideoRendererId(Utils::VideoRendererId::GLWidget2));
        else if ((rendererOptionValue == QLatin1String("qt")) &&
                 (SettingsManager::getInstance()->getRenderer() != Utils::getVideoRendererId(Utils::VideoRendererId::Widget)))
            SettingsManager::getInstance()->setRenderer(Utils::getVideoRendererId(Utils::VideoRendererId::Widget));
        else if ((rendererOptionValue == QLatin1String("gdi")) &&
                 (SettingsManager::getInstance()->getRenderer() != Utils::getVideoRendererId(Utils::VideoRendererId::GDI)))
            SettingsManager::getInstance()->setRenderer(Utils::getVideoRendererId(Utils::VideoRendererId::GDI));
        else if ((rendererOptionValue == QLatin1String("d2d")) &&
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
#endif
#ifndef DD_NO_CSS
    SkinsManager::getInstance()->setSkin(SettingsManager::getInstance()->getSkin());
#endif
    PlayerWindow playerWindow;
    PreferencesDialog preferencesDialog;
    AboutDialog aboutDialog;
    PlaylistDialog playlistDialog;
    QSystemTrayIcon trayIcon;
    playerWindow.setWindowMode(windowMode);
#ifndef DD_NO_SVG
    trayIcon.setIcon(QIcon(QStringLiteral(":/icons/color_palette.svg")));
#else
    trayIcon.setIcon(QIcon(QStringLiteral(":/icons/color_palette.png")));
#endif
#ifndef DD_NO_MENU
    TrayMenu trayMenu;
    trayIcon.setContextMenu(&trayMenu);
    QObject::connect(&preferencesDialog, &PreferencesDialog::muteChanged, &trayMenu, &TrayMenu::setMute);
    QObject::connect(&preferencesDialog, &PreferencesDialog::about, &trayMenu, &TrayMenu::onAboutClicked);
    QObject::connect(&playerWindow, &PlayerWindow::playStateChanged, &trayMenu, &TrayMenu::setPlaying);
    QObject::connect(&trayMenu, &TrayMenu::onOptionsClicked, [=, &preferencesDialog]
    {
        Utils::activateWindow(&preferencesDialog);
    });
    QObject::connect(&trayMenu, &TrayMenu::onPreviousClicked, &preferencesDialog, &PreferencesDialog::playPreviousMedia);
    QObject::connect(&trayMenu, &TrayMenu::onPlayClicked, &preferencesDialog, &PreferencesDialog::togglePlayPause);
    QObject::connect(&trayMenu, &TrayMenu::onNextClicked, &preferencesDialog, &PreferencesDialog::playNextMedia);
    trayMenu.setMute(SettingsManager::getInstance()->getMute());
    QObject::connect(&trayMenu, &TrayMenu::onMuteClicked, [=, &preferencesDialog]
    {
        preferencesDialog.setMute(!SettingsManager::getInstance()->getMute());
    });
    QObject::connect(&trayMenu, &TrayMenu::onAboutClicked, [=, &aboutDialog]
    {
        Utils::activateWindow(&aboutDialog);
    });
    QObject::connect(&trayMenu, &TrayMenu::onExitClicked, &app, &QtSingleApplication::quit);
    QObject::connect(&app, &QtSingleApplication::messageReceived, [=, &trayMenu](const QString &message)
    {
        Q_UNUSED(message)
        emit trayMenu.onOptionsClicked();
    });
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, [=, &trayMenu](QSystemTrayIcon::ActivationReason reason)
    {
        if (reason != QSystemTrayIcon::Context)
            emit trayMenu.onOptionsClicked();
    });
#ifndef DD_NO_TRANSLATIONS
    QObject::connect(&preferencesDialog, &PreferencesDialog::languageChanged, [=, &preferencesDialog, &playlistDialog, &aboutDialog, &trayMenu, &ddTranslator](const QString &lang)
    {
        installTranslation(lang, ddTranslator);
        preferencesDialog.refreshTexts(lang);
        playlistDialog.refreshTexts(lang);
        aboutDialog.refreshTexts(lang);
        trayMenu.refreshTexts(lang);
        QMessageBox::information(nullptr, QStringLiteral("Dynamic Desktop"), DD_OBJ_TR("Some texts will not refresh their translation until you restart this application."));
    });
#endif
#else
    QObject::connect(&app, &QtSingleApplication::messageReceived, [=, &trayIcon, &preferencesDialog](const QString &message)
    {
        Q_UNUSED(message)
        emit trayIcon.activated(QSystemTrayIcon::DoubleClick);
    });
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, [=, &preferencesDialog](QSystemTrayIcon::ActivationReason reason)
    {
        Q_UNUSED(reason)
        Utils::activateWindow(&preferencesDialog);
    });
#ifndef DD_NO_TRANSLATIONS
    QObject::connect(&preferencesDialog, &PreferencesDialog::languageChanged, [=, &preferencesDialog, &aboutDialog, &ddTranslator](const QString &lang)
    {
        installTranslation(lang, ddTranslator);
        preferencesDialog.refreshTexts(lang);
        aboutDialog.refreshTexts(lang);
        QMessageBox::information(nullptr, QStringLiteral("Dynamic Desktop"), DD_OBJ_TR("Some texts will not refresh their translation until you restart this application."));
    });
#endif
#endif
    QObject::connect(&preferencesDialog, &PreferencesDialog::showPlaylistDialog, [=, &playlistDialog]
    {
        Utils::activateWindow(&playlistDialog);
    });
    QObject::connect(&preferencesDialog, &PreferencesDialog::play, &playerWindow, &PlayerWindow::play);
    QObject::connect(&preferencesDialog, &PreferencesDialog::pause, &playerWindow, &PlayerWindow::pause);
    QObject::connect(&preferencesDialog, &PreferencesDialog::urlChanged, &playerWindow, &PlayerWindow::setUrl);
#ifndef DD_NO_TOOLTIP
    QObject::connect(&preferencesDialog, &PreferencesDialog::urlChanged, [=, &trayIcon](const QString &text)
    {
        trayIcon.setToolTip(QStringLiteral("Dynamic Desktop: %0").arg(text));
    });
#endif
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
    QObject::connect(&preferencesDialog, &PreferencesDialog::repeatCurrentFile, &playerWindow, &PlayerWindow::setRepeatCurrentFile);
    QObject::connect(&playerWindow, &PlayerWindow::playStateChanged, &preferencesDialog, &PreferencesDialog::setPlaying);
    QObject::connect(&playerWindow, &PlayerWindow::mediaEndReached, &preferencesDialog, &PreferencesDialog::mediaEndReached);
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
    QObject::connect(&playlistDialog, &PlaylistDialog::dataRefreshed, &preferencesDialog, &PreferencesDialog::refreshPlaylistsAndFiles);
    QObject::connect(&playlistDialog, &PlaylistDialog::switchPlaylist, &preferencesDialog, &PreferencesDialog::switchPlaylist);
    QObject::connect(&playlistDialog, &PlaylistDialog::playFile, &preferencesDialog, &PreferencesDialog::switchFile);
    QObject::connect(qApp, &QtSingleApplication::aboutToQuit, [=]
    {
        Wallpaper::hideWallpaper();
    });
    trayIcon.show();
    const Qt::WindowFlags windowFlags = Qt::FramelessWindowHint;
    const QRect screenGeometry = QtSingleApplication::desktop()->screenGeometry(&playerWindow);
    if (!windowMode)
    {
        playerWindow.setWindowFlags(windowFlags);
        // Why is Direct2D image too large?
        playerWindow.setGeometry(screenGeometry);
        // How to place our window under desktop icons:
        // Use "Program Manager" as our parent window in Win7/8/8.1.
        // Use "WorkerW" as our parent window in Win10.
        // Use "Program Manager" as our parent window in
        // Win10 is also OK, but our window will come
        // to front if we press "Win + Tab" and it will
        // also block our desktop icons, however using
        // "WorkerW" as our parent window will not result
        // in this problem, I don't know why. It's strange.
        Wallpaper::setLegacyMode(QOperatingSystemVersion::current() < QOperatingSystemVersion::Windows10);
        Wallpaper::setWallpaper(reinterpret_cast<HWND>(playerWindow.winId()));
    }
    else
    {
        playerWindow.resize(1280, 720);
        Utils::activateWindow(&playerWindow, false);
    }
    if (SettingsManager::getInstance()->getLastFile().isEmpty())
    {
        Utils::activateWindow(&preferencesDialog);
#ifndef DD_NO_TOOLTIP
        trayIcon.setToolTip(QStringLiteral("Dynamic Desktop"));
#endif
    }
    else
    {
        const QString url = SettingsManager::getInstance()->getLastFile();
        Utils::activateWindow(&playerWindow, false);
        playerWindow.setUrl(url);
#ifndef DD_NO_TOOLTIP
        trayIcon.setToolTip(QStringLiteral("Dynamic Desktop: %0").arg(url));
#endif
    }
    return QtSingleApplication::exec();
}
