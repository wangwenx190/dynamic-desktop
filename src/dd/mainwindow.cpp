#include "mainwindow.h"
#include "utils.h"
#include "settingsmanager.h"
#include "skinmanager.h"
#include "forms/aboutdialog.h"
#include "forms/preferencesdialog.h"

#include <QMessageBox>
#include <QApplication>
#include <QTimer>
#include <QVBoxLayout>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QAction>
#include <QtAV>
#include <QtAVWidgets>

static bool firstLoad = true;
const qreal kVolumeInterval = 0.04;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    initUI();
    initPlayer();
    initConnections();
    if (player->audio())
    {
        preferencesDialog->volumeChanged(SettingsManager::getInstance()->getVolume());
        player->audio()->setMute(SettingsManager::getInstance()->getMute());
        muteAction->setCheckable(true);
        muteAction->setChecked(SettingsManager::getInstance()->getMute());
    }
    else
    {
        muteAction->setCheckable(false);
        muteAction->setEnabled(false);
        preferencesDialog->setVolumeAreaEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete aboutDialog;
    delete preferencesDialog;
    delete trayIcon;
    delete trayMenu;
    delete subtitle;
    delete renderer;
    delete player;
    delete mainLayout;
}

void MainWindow::initUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setWindowIcon(QIcon(QStringLiteral(":/images/bee.ico")));
    setWindowTitle(QStringLiteral("Dynamic Desktop"));
    preferencesDialog = new PreferencesDialog();
    aboutDialog = new AboutDialog();
    trayMenu = new QMenu(this);
    trayMenu->addAction(tr("Preferences"), this, &MainWindow::showPreferencesDialog);
    trayMenu->addSeparator();
    trayMenu->addAction(tr("Play"), this, static_cast<void(MainWindow::*)(void)>(&MainWindow::play));
    trayMenu->addAction(tr("Pause"), this, &MainWindow::pause);
    muteAction = trayMenu->addAction(tr("Mute"));
    muteAction->setCheckable(true);
    trayMenu->addSeparator();
    trayMenu->addAction(tr("About"), this, &MainWindow::showAboutDialog);
    trayMenu->addAction(tr("Exit"), qApp, &QApplication::closeAllWindows);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(QStringLiteral(":/images/bee.ico")));
    trayIcon->setToolTip(QStringLiteral("Dynamic Desktop"));
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
}

void MainWindow::initPlayer()
{
    player = new QtAV::AVPlayer(this);
    player->setRepeat(-1);
    subtitle = new QtAV::SubtitleFilter(this);
    subtitle->setPlayer(player);
    subtitle->setCodec(SettingsManager::getInstance()->getCharset().toLatin1());
    subtitle->setEngines(QStringList() << QStringLiteral("LibASS") << QStringLiteral("FFmpeg"));
    subtitle->setAutoLoad(SettingsManager::getInstance()->getSubtitleAutoLoad());
    subtitle->setEnabled(SettingsManager::getInstance()->getSubtitle());
    QtAV::VideoRenderer *videoOutput = QtAV::VideoRenderer::create(SettingsManager::getInstance()->getRenderer());
    setRenderer(videoOutput);
    setImageQuality();
    setImageRatio();
}

void MainWindow::initConnections()
{
    connect(player, &QtAV::AVPlayer::started,
        [=]
        {
            QTimer::singleShot(0, [=]{ onStartPlay(); });
        });
    connect(muteAction, &QAction::triggered,
        [=](bool checked)
        {
            if (player->audio())
            {
                muteAction->setChecked(checked);
                SettingsManager::getInstance()->setMute(checked);
                preferencesDialog->updateVolumeArea();
                QTimer::singleShot(0, [=]{ player->audio()->setMute(checked); });
            }
        });
    connect(trayIcon, &QSystemTrayIcon::activated,
        [=](QSystemTrayIcon::ActivationReason reason)
        {
            if (reason != QSystemTrayIcon::Context)
                QTimer::singleShot(0, [=]{ showPreferencesDialog(); });
        });
    connect(preferencesDialog, &PreferencesDialog::about,
        [=]
        {
            QTimer::singleShot(0, [=]{ showAboutDialog(); });
        });
    connect(preferencesDialog, &PreferencesDialog::pause,
        [=]
        {
            QTimer::singleShot(0, [=]{ pause(); });
        });
    connect(preferencesDialog, &PreferencesDialog::urlChanged,
        [=](const QString &url)
        {
            QTimer::singleShot(0, [=]{ urlChanged(url); });
        });
    connect(preferencesDialog, &PreferencesDialog::volumeChanged,
        [=](unsigned int volume)
        {
            QtAV::AudioOutput *ao = player ? player->audio() : nullptr;
            if (ao)
            {
                qreal newVolume = static_cast<qreal>(volume) * kVolumeInterval;
                if (ao->volume() != newVolume)
                {
                    if (qAbs(static_cast<unsigned int>(ao->volume() / kVolumeInterval) - volume) >= static_cast<unsigned int>(0.1 / kVolumeInterval))
                        QTimer::singleShot(0, [=]{ ao->setVolume(newVolume); });
                    preferencesDialog->setVolumeToolTip(tr("Volume: %0").arg(QString::number(newVolume)));
                }
            }
        });
    connect(preferencesDialog, &PreferencesDialog::muteChanged,
        [=](bool mute)
        {
            if (player->audio())
                if (player->audio()->isMute() != mute)
                    QTimer::singleShot(0, [=]{ muteAction->triggered(mute); });
        });
    connect(preferencesDialog, &PreferencesDialog::seekBySlider,
        [=](qint64 value)
        {
            if (player->isLoaded() && player->isSeekable())
                QTimer::singleShot(0, [=]{ player->seek(value); });
        });
    connect(preferencesDialog, &PreferencesDialog::pictureRatioChanged, this, static_cast<void(MainWindow::*)(bool)>(&MainWindow::setImageRatio));
    connect(preferencesDialog, &PreferencesDialog::videoTrackChanged,
        [=](unsigned int id)
        {
            if (player->isLoaded())
                if (id != player->currentVideoStream())
                    QTimer::singleShot(0, [=]{ player->setVideoStream(id); });
        });
    connect(preferencesDialog, &PreferencesDialog::audioTrackChanged,
        [=](unsigned int id)
        {
            if (player->isLoaded())
                if (id != player->currentAudioStream())
                    QTimer::singleShot(0, [=]{ player->setAudioStream(id); });
        });
    connect(preferencesDialog, &PreferencesDialog::subtitleTrackChanged,
        [=](const QVariant &track)
        {
            if (player->isLoaded())
            {
                const QString newSubFile = track.toString();
                if (QFileInfo::exists(newSubFile) && subtitle->file() != newSubFile)
                    QTimer::singleShot(0, [=]{ subtitle->setFile(newSubFile); });
                else
                {
                    unsigned int id = track.toUInt();
                    if (id != player->currentSubtitleStream())
                        QTimer::singleShot(0, [=]{ player->setSubtitleStream(id); });
                }
            }
        });
    connect(preferencesDialog, &PreferencesDialog::subtitleOpened,
        [=](const QString &subPath)
        {
            if (player->isLoaded())
                if (subtitle->file() != subPath)
                    QTimer::singleShot(0, [=]{ subtitle->setFile(subPath); });
        });
    connect(preferencesDialog, &PreferencesDialog::audioOpened,
        [=](const QString &audioPath)
        {
            if (player->isLoaded() && player->audio())
                QTimer::singleShot(0, [=]{ player->setExternalAudio(audioPath); });
        });
    connect(preferencesDialog, &PreferencesDialog::charsetChanged,
        [=](const QString &charset)
        {
            if (SettingsManager::getInstance()->getSubtitle())
                if (subtitle->codec() != charset.toLatin1())
                    QTimer::singleShot(0, [=]{ subtitle->setCodec(charset.toLatin1()); });
        });
    connect(preferencesDialog, &PreferencesDialog::subtitleAutoLoadChanged,
        [=](bool autoload)
        {
            if (subtitle->autoLoad() != autoload)
                QTimer::singleShot(0, [=]{ subtitle->setAutoLoad(autoload); });
        });
    connect(preferencesDialog, &PreferencesDialog::subtitleEnabled,
        [=](bool enabled)
        {
            if (subtitle->isEnabled() != enabled)
                QTimer::singleShot(0, [=]{ subtitle->setEnabled(enabled); });
        });
    connect(preferencesDialog, &PreferencesDialog::skinChanged,
        [=](const QString &skin)
        {
            if (SkinManager::getInstance()->currentSkinName() != skin)
                QTimer::singleShot(0, [=]{ SkinManager::getInstance()->setSkin(skin); });
        });
    connect(preferencesDialog, &PreferencesDialog::rendererChanged,
        [=](QtAV::VideoRendererId rendererId)
        {
            if ((rendererId != renderer->id()) || !renderer)
                QTimer::singleShot(0,
                    [=]
                    {
                        QtAV::VideoRenderer *newRenderer = QtAV::VideoRenderer::create(rendererId);
                        setRenderer(newRenderer);
                    });
        });
    connect(preferencesDialog, &PreferencesDialog::imageQualityChanged,
        [=](const QString &quality)
        {
            QTimer::singleShot(0, [=]{ setImageQuality(quality); });
        });
    connect(player, &QtAV::AVPlayer::positionChanged,
        [=](qint64 pos)
        {
            QTimer::singleShot(0,
                [=]
                {
                    preferencesDialog->updateVideoSlider(pos);
                    preferencesDialog->setVideoPositionText(QTime(0, 0, 0).addMSecs(pos).toString(QStringLiteral("HH:mm:ss")));
                });
        });
    connect(this, &MainWindow::showOptions,
        [=]
        {
            QTimer::singleShot(0, [=]{ showPreferencesDialog(); });
        });
    connect(this, static_cast<void(MainWindow::*)(const QString &)>(&MainWindow::play),
        [=](const QString &url)
        {
            QTimer::singleShot(0, [=]{ urlChanged(url); });
        });
}

void MainWindow::disconnectAll()
{
    disconnect(player, &QtAV::AVPlayer::started, nullptr, nullptr);
    disconnect(muteAction, &QAction::triggered, nullptr, nullptr);
    disconnect(trayIcon, &QSystemTrayIcon::activated, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::about, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::pause, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::urlChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::volumeChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::muteChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::seekBySlider, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::pictureRatioChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::videoTrackChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::audioTrackChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::subtitleTrackChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::subtitleOpened, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::audioOpened, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::charsetChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::subtitleAutoLoadChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::subtitleEnabled, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::skinChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::rendererChanged, nullptr, nullptr);
    disconnect(preferencesDialog, &PreferencesDialog::imageQualityChanged, nullptr, nullptr);
    disconnect(player, &QtAV::AVPlayer::positionChanged, nullptr, nullptr);
    disconnect(this, &MainWindow::showOptions, nullptr, nullptr);
    disconnect(this, static_cast<void(MainWindow::*)(const QString &)>(&MainWindow::play), nullptr, nullptr);
}

void MainWindow::refreshPlayer()
{
    disconnectAll();
    if (subtitle)
    {
        delete subtitle;
        subtitle = nullptr;
    }
    QWidget *oldRendererWidget = nullptr;
    if (renderer)
        oldRendererWidget = renderer->widget();
    if (oldRendererWidget)
    {
        mainLayout->removeWidget(oldRendererWidget);
        if (oldRendererWidget->testAttribute(Qt::WA_DeleteOnClose))
            oldRendererWidget->close();
        else
        {
            oldRendererWidget->close();
            delete oldRendererWidget;
        }
        oldRendererWidget = nullptr;
    }
    if (renderer)
    {
        // deleting "renderer" will crash
        // currently don't know why
        //delete renderer;
        renderer = nullptr;
    }
    if (player)
    {
        delete player;
        player = nullptr;
    }
    initPlayer();
    initConnections();
}

bool MainWindow::setRenderer(QtAV::VideoRenderer *videoRenderer)
{
    if (!player || !subtitle)
        return false;
    if (!videoRenderer || !videoRenderer->isAvailable() || !videoRenderer->widget())
    {
        QMessageBox::critical(nullptr, QStringLiteral("Dynamic Desktop"), tr("Current renderer is not available on your platform!"));
        return false;
    }
    subtitle->uninstall();
    player->setRenderer(videoRenderer);
    QWidget *oldRendererWidget = nullptr;
    if (renderer)
        oldRendererWidget = renderer->widget();
    if (oldRendererWidget)
    {
        mainLayout->removeWidget(oldRendererWidget);
        if (oldRendererWidget->testAttribute(Qt::WA_DeleteOnClose))
            oldRendererWidget->close();
        else
        {
            oldRendererWidget->close();
            delete oldRendererWidget;
        }
        oldRendererWidget = nullptr;
    }
    renderer = videoRenderer;
    mainLayout->addWidget(renderer->widget());
    const QtAV::VideoDecoderId vid = renderer->id();
    if (vid == QtAV::VideoRendererId_GLWidget
            || vid == QtAV::VideoRendererId_GLWidget2
            || vid == QtAV::VideoRendererId_OpenGLWidget)
        renderer->forcePreferredPixelFormat(true);
    else
        renderer->forcePreferredPixelFormat(false);
    subtitle->installTo(renderer);
    return true;
}

void MainWindow::setImageQuality(const QString &quality)
{
    if (!renderer)
        return;
    if ((quality == QStringLiteral("default")) &&
            (renderer->quality() != QtAV::VideoRenderer::QualityDefault))
        renderer->setQuality(QtAV::VideoRenderer::QualityDefault);
    else if ((quality == QStringLiteral("best")) &&
             (renderer->quality() != QtAV::VideoRenderer::QualityBest))
        renderer->setQuality(QtAV::VideoRenderer::QualityBest);
    else if ((quality == QStringLiteral("fastest")) &&
             (renderer->quality() != QtAV::VideoRenderer::QualityFastest))
        renderer->setQuality(QtAV::VideoRenderer::QualityFastest);
}

void MainWindow::setImageQuality()
{
    setImageQuality(SettingsManager::getInstance()->getImageQuality());
}

void MainWindow::setImageRatio(bool fit)
{
    if (!renderer)
        return;
    if (fit && (renderer->outAspectRatioMode() != QtAV::VideoRenderer::RendererAspectRatio))
        renderer->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
    else if (!fit && (renderer->outAspectRatioMode() != QtAV::VideoRenderer::VideoAspectRatio))
        renderer->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
}

void MainWindow::setImageRatio()
{
    setImageRatio(SettingsManager::getInstance()->getFitDesktop());
}

void MainWindow::showAboutDialog()
{
    if (!aboutDialog)
        aboutDialog = new AboutDialog();
    if (aboutDialog->isHidden())
    {
        Utils::moveToCenter(aboutDialog);
        aboutDialog->show();
    }
    if (!aboutDialog->isActiveWindow())
        aboutDialog->setWindowState(aboutDialog->windowState() & ~Qt::WindowMinimized);
    if (!aboutDialog->isActiveWindow())
    {
        aboutDialog->raise();
        aboutDialog->activateWindow();
    }
}

void MainWindow::showPreferencesDialog()
{
    if (!preferencesDialog)
        preferencesDialog = new PreferencesDialog();
    if (preferencesDialog->isHidden())
    {
        Utils::moveToCenter(preferencesDialog);
        preferencesDialog->show();
    }
    if (!preferencesDialog->isActiveWindow())
        preferencesDialog->setWindowState(preferencesDialog->windowState() & ~Qt::WindowMinimized);
    if (!preferencesDialog->isActiveWindow())
    {
        preferencesDialog->raise();
        preferencesDialog->activateWindow();
    }
}

void MainWindow::onStartPlay()
{
    if (!preferencesDialog || !player || !subtitle)
        return;
    preferencesDialog->clearAllTracks();
    preferencesDialog->updateVideoSliderUnit(player->notifyInterval());
    preferencesDialog->updateVideoSliderRange(player->duration());
    preferencesDialog->updateVideoSlider(player->position());
    preferencesDialog->setSeekAreaEnabled(player->isSeekable());
    preferencesDialog->setAudioAreaEnabled(player->audio());
    preferencesDialog->updateVideoTracks(player->internalVideoTracks());
    preferencesDialog->updateAudioTracks(player->internalAudioTracks(), false);
    preferencesDialog->setVideoDurationText(QTime(0, 0, 0).addMSecs(player->mediaStopPosition()).toString(QStringLiteral("HH:mm:ss")));
    if (SettingsManager::getInstance()->getAudioAutoLoad())
        preferencesDialog->updateAudioTracks(player->externalAudioTracks(), true);
    preferencesDialog->updateSubtitleTracks(player->internalSubtitleTracks(), false);
    if (SettingsManager::getInstance()->getSubtitleAutoLoad())
    {
        QVariantList externalSubtitleTracks;
        QStringList externalSubtitlePaths = Utils::externalFilesToLoad(QFileInfo(player->file()), QStringLiteral("sub"));
        if (!externalSubtitlePaths.isEmpty())
        {
            for (auto& subPath : externalSubtitlePaths)
            {
                QVariantMap externalSubtitle;
                externalSubtitle[QStringLiteral("file")] = subPath;
                externalSubtitleTracks.append(externalSubtitle);
            }
            preferencesDialog->updateSubtitleTracks(externalSubtitleTracks, true);
        }
    }
    subtitle->setEnabled(false);
    if (SettingsManager::getInstance()->getSubtitle())
    {
        if (player->subtitleStreamCount() > 0)
        {
            subtitle->setEnabled(true);
            player->setSubtitleStream(0);
        }
        else if (SettingsManager::getInstance()->getSubtitleAutoLoad())
        {
            QStringList externalSubtitles = Utils::externalFilesToLoad(QFileInfo(player->file()), QStringLiteral("sub"));
            if (!externalSubtitles.isEmpty())
            {
                subtitle->setEnabled(true);
                if (subtitle->file() != externalSubtitles.constFirst())
                    subtitle->setFile(externalSubtitles.constFirst());
            }
        }
    }
}

void MainWindow::play()
{
    if (!player)
        return;
    if (player->isPaused())
        player->pause(false);
}

void MainWindow::pause()
{
    if (!player)
        return;
    if (player->isPlaying())
        player->pause();
}

void MainWindow::stop()
{
    if (!player)
        return;
    if (player->isLoaded())
        player->stop();
}

void MainWindow::urlChanged(const QString &url)
{
    if (!player)
        return;
    if (!url.isEmpty())
        stop();
    if (!firstLoad && !url.isEmpty())
        refreshPlayer();
    if (SettingsManager::getInstance()->getHwdec())
    {
        QStringList decoders = SettingsManager::getInstance()->getDecoders();
        if (!decoders.contains(QStringLiteral("FFmpeg")))
            decoders << QStringLiteral("FFmpeg");
        if (player->videoDecoderPriority() != decoders)
            player->setVideoDecoderPriority(decoders);
        if (decoders.contains(QStringLiteral("CUDA")))
        {
            QVariantHash cuda_opt;
            cuda_opt[QStringLiteral("surfaces")] = 0;
            cuda_opt[QStringLiteral("copyMode")] = QStringLiteral("ZeroCopy");
            QVariantHash opt;
            opt[QStringLiteral("CUDA")] = cuda_opt;
            player->setOptionsForVideoCodec(opt);
        }
        if (decoders.contains(QStringLiteral("D3D11")))
        {
            QVariantHash d3d11_opt;
            //d3d11_opt[QStringLiteral("???")] = ???;
            d3d11_opt[QStringLiteral("copyMode")] = QStringLiteral("ZeroCopy");
            QVariantHash opt;
            opt[QStringLiteral("D3D11")] = d3d11_opt;
            player->setOptionsForVideoCodec(opt);
        }
        if (decoders.contains(QStringLiteral("DXVA")))
        {
            QVariantHash dxva_opt;
            //dxva_opt[QStringLiteral("???")] = ???;
            dxva_opt[QStringLiteral("copyMode")] = QStringLiteral("ZeroCopy");
            QVariantHash opt;
            opt[QStringLiteral("DXVA")] = dxva_opt;
            player->setOptionsForVideoCodec(opt);
        }
    }
    else if (player->videoDecoderPriority() != (QStringList() << QStringLiteral("FFmpeg")))
        player->setVideoDecoderPriority(QStringList() << QStringLiteral("FFmpeg"));
    if (isHidden())
        show();
    if (!url.isEmpty())
    {
        QTimer::singleShot(0, [=]{ player->play(url); });
        setWindowTitle(QFileInfo(url).fileName());
    }
    else
        play();
    preferencesDialog->volumeChanged(SettingsManager::getInstance()->getVolume());
    if (firstLoad)
        firstLoad = false;
}
