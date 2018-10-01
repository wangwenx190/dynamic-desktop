#include "forms/preferencesdialog.h"
#include "forms/aboutdialog.h"
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
#ifndef STATIC
#include <QLibraryInfo>
#endif
#include <QSysInfo>
#include <QVersionNumber>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QDir>

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

QStringList externalFilesToLoad(const QFileInfo &originalMediaFile, const QString &fileType)
{
    if (!originalMediaFile.exists() || originalMediaFile.isDir() || fileType.isEmpty())
        return QStringList();
    QDir subDir(originalMediaFile.absoluteDir());
    QFileInfoList fileList = subDir.entryInfoList(QDir::Files | QDir::NoSymLinks, QDir::Name);
    if (fileList.count() < 2)
        return QStringList();
    const QString fileBaseName = originalMediaFile.baseName().toLower();
    QStringList newFileList;
    for (auto& fi : fileList)
    {
        if (fi.absoluteFilePath() == originalMediaFile.absoluteFilePath())
            continue;
        const QString newBaseName = fi.baseName().toLower();
        if (newBaseName == fileBaseName)
            if (fileType.toLower() == QLatin1String("sub"))
            {
                if (fi.suffix().toLower() == QLatin1String("ass")
                        || fi.suffix().toLower() == QLatin1String("ssa")
                        || fi.suffix().toLower() == QLatin1String("srt")
                        || fi.suffix().toLower() == QLatin1String("sup"))
                    newFileList.append(QDir::toNativeSeparators(fi.absoluteFilePath()));
            }
            else if (fileType.toLower() == QLatin1String("audio"))
                if (fi.suffix().toLower() == QLatin1String("mka"))
                    newFileList.append(QDir::toNativeSeparators(fi.absoluteFilePath()));
    }
    return newFileList;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("Dynamic Desktop"));
    QApplication::setApplicationDisplayName(QStringLiteral("Dynamic Desktop"));
    QCoreApplication::setApplicationVersion(QStringLiteral(DD_VERSION));
    QCoreApplication::setOrganizationName(QStringLiteral("wangwenx190"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("wangwenx190.github.io"));
    QTranslator ddTranslator;
    if (SettingsManager::getInstance()->getLocalize())
    {
        QString qmDir;
#ifdef STATIC
        qmDir = QStringLiteral(":/i18n");
#else
        qmDir = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
        if (ddTranslator.load(QLocale(), QStringLiteral("dd"), QStringLiteral("_"), qmDir))
            QApplication::installTranslator(&ddTranslator);
    }
    int suffixIndex;
    QVersionNumber currentVersion = QVersionNumber::fromString(QSysInfo::kernelVersion(), &suffixIndex);
    QVersionNumber win7Version(6, 1, 7600);
    if (currentVersion < win7Version)
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("This application only supports Windows 7 and newer."));
        return 0;
    }
    HANDLE mutex = CreateMutex(nullptr, FALSE, TEXT("wangwenx190.DynamicDesktop.1000.AppMutex"));
    if ((mutex != nullptr) && (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("There is another instance running. Please do not run twice."));
        ReleaseMutex(mutex);
        return 0;
    }
    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("A tool that make your desktop alive."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);
#ifndef STATIC
    if (QLibraryInfo::isDebugBuild())
        QMessageBox::warning(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("WARNING: You are running a debug version of this tool!\nDo not continue running it if you are not a developer!"));
#endif
    if (SettingsManager::getInstance()->getAutostart())
        SettingsManager::getInstance()->regAutostart();
    else
        SettingsManager::getInstance()->unregAutostart();
    QtAV::GLWidgetRenderer2 renderer;
    renderer.forcePreferredPixelFormat(true);
    renderer.setQuality(QtAV::VideoRenderer::QualityFastest);
    if (SettingsManager::getInstance()->getFitDesktop())
        renderer.setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
    else
        renderer.setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
    renderer.setWindowIcon(QIcon(QStringLiteral(":/bee.ico")));
    renderer.setWindowTitle(QObject::tr("My wallpaper"));
    renderer.setAttribute(Qt::WA_NoSystemBackground);
    renderer.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint | Qt::WindowDoesNotAcceptFocus);
    QRect screenGeometry = QApplication::desktop()->screenGeometry(&renderer);
    renderer.setGeometry(screenGeometry);
    QtAV::AVPlayer player;
    player.setRenderer(&renderer);
    QtAV::SubtitleFilter subtitle;
    subtitle.setPlayer(&player);
    subtitle.installTo(&renderer);
    subtitle.setCodec(SettingsManager::getInstance()->getCharset().toLatin1());
    subtitle.setEngines(QStringList() << QStringLiteral("LibASS") << QStringLiteral("FFmpeg"));
    subtitle.setAutoLoad(SettingsManager::getInstance()->getSubtitleAutoLoad());
    subtitle.setEnabled(SettingsManager::getInstance()->getSubtitle());
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
        if (decoders.contains(QStringLiteral("D3D11")))
        {
            QVariantHash d3d11_opt;
            //d3d11_opt[QStringLiteral("???")] = ???;
            d3d11_opt[QStringLiteral("copyMode")] = QStringLiteral("ZeroCopy");
            QVariantHash opt;
            opt[QStringLiteral("D3D11")] = d3d11_opt;
            player.setOptionsForVideoCodec(opt);
        }
        if (decoders.contains(QStringLiteral("DXVA")))
        {
            QVariantHash dxva_opt;
            //dxva_opt[QStringLiteral("???")] = ???;
            dxva_opt[QStringLiteral("copyMode")] = QStringLiteral("ZeroCopy");
            QVariantHash opt;
            opt[QStringLiteral("DXVA")] = dxva_opt;
            player.setOptionsForVideoCodec(opt);
        }
    }
    player.setRepeat(-1);
    PreferencesDialog preferencesDialog;
    AboutDialog aboutDialog;
    QObject::connect(&player, SIGNAL(stopped()), &player, SLOT(play()));
    QObject::connect(&player, SIGNAL(positionChanged(qint64)), &preferencesDialog, SIGNAL(updateVideoSlider(qint64)));
    QObject::connect(&player, &QtAV::AVPlayer::loaded,
        [=, &preferencesDialog, &player, &subtitle]
        {
            preferencesDialog.clearAllTracks();
            preferencesDialog.updateVideoSliderUnit(player.notifyInterval());
            preferencesDialog.updateVideoSliderRange(player.duration());
            preferencesDialog.updateVideoSlider(player.position());
            preferencesDialog.setSeekAreaEnabled(player.isSeekable());
            preferencesDialog.setAudioAreaEnabled(player.audio());
            preferencesDialog.updateVideoTracks(player.internalVideoTracks());
            preferencesDialog.updateAudioTracks(player.internalAudioTracks(), false);
            if (SettingsManager::getInstance()->getAudioAutoLoad())
                preferencesDialog.updateAudioTracks(player.externalAudioTracks(), true);
            preferencesDialog.updateSubtitleTracks(player.internalSubtitleTracks(), false);
            if (SettingsManager::getInstance()->getSubtitleAutoLoad())
            {
                QVariantList externalSubtitleTracks;
                QStringList externalSubtitlePaths = externalFilesToLoad(QFileInfo(player.file()), QStringLiteral("sub"));
                if (!externalSubtitlePaths.isEmpty())
                {
                    for (auto& subPath : externalSubtitlePaths)
                    {
                        QVariantMap externalSubtitle;
                        externalSubtitle[QStringLiteral("file")] = subPath;
                        externalSubtitleTracks.append(externalSubtitle);
                    }
                    preferencesDialog.updateSubtitleTracks(externalSubtitleTracks, true);
                }
            }
            if (SettingsManager::getInstance()->getSubtitle() && SettingsManager::getInstance()->getSubtitleAutoLoad() && subtitle.file().isEmpty())
            {
                QStringList externalSubtitles = externalFilesToLoad(QFileInfo(player.file()), QStringLiteral("sub"));
                if (!externalSubtitles.isEmpty())
                    subtitle.setFile(externalSubtitles.constFirst());
            }
        });
    QObject::connect(&player, &QtAV::AVPlayer::notifyIntervalChanged,
        [=, &preferencesDialog, &player]
        {
            preferencesDialog.updateVideoSliderUnit(player.notifyInterval());
            preferencesDialog.updateVideoSlider(player.position());
        });
    QObject::connect(&player, &QtAV::AVPlayer::durationChanged,
        [=, &preferencesDialog, &player](qint64 duration)
        {
            preferencesDialog.updateVideoSliderRange(duration);
            preferencesDialog.updateVideoSlider(player.position());
        });
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
            if (player.isLoaded())
            {
                if (renderer.isHidden())
                    renderer.show();
                player.play();
            }
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
                SettingsManager::getInstance()->setMute(checked);
                preferencesDialog.updateVolumeArea();
                player.audio()->setMute(checked);
            }
        });
    trayMenu.addSeparator();
    QAction *aboutAction = trayMenu.addAction(QObject::tr("About"));
    QObject::connect(aboutAction, SIGNAL(triggered()), &aboutDialog, SLOT(show()));
    trayMenu.addAction(QObject::tr("Exit"), qApp, &QApplication::closeAllWindows);
    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(QStringLiteral(":/bee.ico")));
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
        preferencesDialog.setVolumeAreaEnabled(false);
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
    QObject::connect(&preferencesDialog, &PreferencesDialog::seekBySlider,
        [=, &player](qint64 value)
        {
            if (player.isPlaying())
                player.seek(value);
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::pictureRatioChanged,
        [=, &renderer](bool fitDesktop)
        {
            if (fitDesktop)
                renderer.setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
            else
                renderer.setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::videoTrackChanged,
        [=, &player](int id)
        {
            if (player.isLoaded())
            {
                SettingsManager::getInstance()->setCurrentVideoStream(id);
                if (id != player.currentVideoStream())
                    player.setVideoStream(id);
            }
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::audioTrackChanged,
        [=, &player](int id)
        {
            if (player.isLoaded())
            {
                SettingsManager::getInstance()->setCurrentAudioStream(id);
                if (id != player.currentAudioStream())
                    player.setAudioStream(id);
            }
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::subtitleTrackChanged,
        [=, &player, &subtitle](const QVariant &track)
        {
            if (player.isLoaded())
            {
                const QString newSubFile = track.toString();
                SettingsManager::getInstance()->setCurrentSubtitleStream(newSubFile);
                if (QFileInfo::exists(newSubFile) && subtitle.file() != newSubFile)
                    subtitle.setFile(newSubFile);
                else
                {
                    int id = track.toInt();
                    SettingsManager::getInstance()->setCurrentSubtitleStream(id);
                    if (id != player.currentSubtitleStream())
                        player.setSubtitleStream(id);
                }
            }
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::charsetChanged,
        [=, &subtitle](const QString &charset)
        {
            if (SettingsManager::getInstance()->getSubtitle())
                subtitle.setCodec(charset.toLatin1());
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::subtitleAutoLoadChanged,
        [=, &subtitle](bool autoload)
        {
            subtitle.setAutoLoad(autoload);
        });
    QObject::connect(&preferencesDialog, &PreferencesDialog::subtitleEnabled,
        [=, &subtitle](bool enabled)
        {
            subtitle.setEnabled(enabled);
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
    else
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), QObject::tr("Cannot get \"Program Manager\"'s handle. Application aborting."));
        ReleaseMutex(mutex);
        CloseHandle(mutex);
        return 0;
    }
    int exec = QApplication::exec();
    ShowWindow(HWORKERW, SW_HIDE);
    ReleaseMutex(mutex);
    CloseHandle(mutex);
    return exec;
}
