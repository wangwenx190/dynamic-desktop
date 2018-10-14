#include "mainwindow.h"
#include <utils.h>
#include <settingsmanager.h>
#include <skinsmanager.h>
#include "forms/aboutdialog.h"
#include "forms/preferencesdialog.h"

#include <QMessageBox>
#include <QApplication>
#include <QtConcurrent>
#include <QVBoxLayout>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QAction>
#include <QtAV>
#include <QtAVWidgets>

const qreal kVolumeInterval = 0.04;
QAction *muteAction = nullptr;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    initUI();
    initPlayer();
    initConnections();
    if (player->audio())
    {
        muteAction->setCheckable(true);
        emit preferencesDialog->volumeChanged(SettingsManager::getInstance()->getVolume());
        emit preferencesDialog->muteChanged(SettingsManager::getInstance()->getMute());
    }
    else
    {
        muteAction->setCheckable(false);
        muteAction->setEnabled(false);
        emit preferencesDialog->setVolumeAreaEnabled(false);
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
    mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    setWindowIcon(QIcon(QStringLiteral(":/images/bee.ico")));
    setWindowTitle(QStringLiteral("Dynamic Desktop"));
    preferencesDialog = new PreferencesDialog();
    aboutDialog = new AboutDialog();
    trayMenu = new QMenu();
    trayMenu->addAction(tr("Preferences"), this, &MainWindow::showPreferencesDialog);
    trayMenu->addSeparator();
    trayMenu->addAction(tr("Play"), this, qOverload<>(&MainWindow::play));
    trayMenu->addAction(tr("Pause"), this, &MainWindow::pause);
    muteAction = trayMenu->addAction(tr("Mute"), preferencesDialog, &PreferencesDialog::muteChanged);
    trayMenu->addSeparator();
    trayMenu->addAction(tr("About"), this, &MainWindow::showAboutDialog);
    trayMenu->addAction(tr("Exit"), qApp, &QApplication::closeAllWindows);
    trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(QStringLiteral(":/images/bee.ico")));
    trayIcon->setToolTip(QStringLiteral("Dynamic Desktop"));
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
}

void MainWindow::initPlayer()
{
    player = new QtAV::AVPlayer();
    player->setRepeat(-1);
    subtitle = new QtAV::SubtitleFilter();
    subtitle->setPlayer(player);
    subtitle->setCodec(SettingsManager::getInstance()->getCharset().toLocal8Bit());
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
    connect(player, &QtAV::AVPlayer::started, this, &MainWindow::onStartPlay);
    connect(trayIcon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason)
    {
        if (reason != QSystemTrayIcon::Context)
            showPreferencesDialog();
    });
    connect(preferencesDialog, &PreferencesDialog::about, this, &MainWindow::showAboutDialog);
    connect(preferencesDialog, &PreferencesDialog::pause, this, &MainWindow::pause);
    connect(preferencesDialog, &PreferencesDialog::urlChanged, this, &MainWindow::urlChanged);
    connect(preferencesDialog, &PreferencesDialog::volumeChanged, this, [=](unsigned int volume)
    {
        QtAV::AudioOutput *ao = player ? player->audio() : nullptr;
        if (ao)
        {
            qreal newVolume = static_cast<qreal>(volume) * kVolumeInterval;
            if (ao->volume() != newVolume)
                if (qAbs(static_cast<unsigned int>(ao->volume() / kVolumeInterval) - volume) >= static_cast<unsigned int>(0.1 / kVolumeInterval))
                    QtConcurrent::run([=]{ ao->setVolume(newVolume); });
            emit preferencesDialog->setVolumeToolTip(tr("Volume: %0").arg(QString::number(newVolume)));
        }
    });
    connect(preferencesDialog, &PreferencesDialog::muteChanged, this, [=](bool mute)
    {
        if (player->audio())
            if (player->audio()->isMute() != mute)
            {
                muteAction->setChecked(mute);
                SettingsManager::getInstance()->setMute(mute);
                emit preferencesDialog->updateVolumeArea();
                QtConcurrent::run([=]{ player->audio()->setMute(mute); });
            }
    });
    connect(preferencesDialog, &PreferencesDialog::seekBySlider, this, [=](qint64 value)
    {
        if (player->isLoaded() && player->isSeekable())
            QtConcurrent::run([=]{ player->seek(value); });
    });
    connect(preferencesDialog, &PreferencesDialog::pictureRatioChanged, this, qOverload<bool>(&MainWindow::setImageRatio));
    connect(preferencesDialog, &PreferencesDialog::videoTrackChanged, this, [=](unsigned int id)
    {
        if (player->isLoaded())
            if (id != player->currentVideoStream())
                QtConcurrent::run([=]{ player->setVideoStream(id); });
    });
    connect(preferencesDialog, &PreferencesDialog::audioTrackChanged, this, [=](unsigned int id)
    {
        if (player->isLoaded())
            if (id != player->currentAudioStream())
                QtConcurrent::run([=]{ player->setAudioStream(id); });
    });
    connect(preferencesDialog, &PreferencesDialog::subtitleTrackChanged, this, [=](const QVariant &track)
    {
        if (player->isLoaded())
        {
            const QString newSubFile = track.toString();
            if (QFileInfo::exists(newSubFile) && subtitle->file() != newSubFile)
                QtConcurrent::run([=]{ subtitle->setFile(newSubFile); });
            else
            {
                unsigned int id = track.toUInt();
                if (id != player->currentSubtitleStream())
                    QtConcurrent::run([=]{ player->setSubtitleStream(id); });
            }
        }
    });
    connect(preferencesDialog, &PreferencesDialog::subtitleOpened, this, [=](const QString &subPath)
    {
        if (player->isLoaded())
            if (subtitle->file() != subPath)
                QtConcurrent::run([=]{ subtitle->setFile(subPath); });
    });
    connect(preferencesDialog, &PreferencesDialog::audioOpened, this, [=](const QString &audioPath)
    {
        if (player->isLoaded() && player->audio())
            QtConcurrent::run([=]{ player->setExternalAudio(audioPath); });
    });
    connect(preferencesDialog, &PreferencesDialog::charsetChanged, this, [=](const QString &charset)
    {
        if (SettingsManager::getInstance()->getSubtitle())
            if (subtitle->codec() != charset.toLocal8Bit())
                QtConcurrent::run([=]{ subtitle->setCodec(charset.toLocal8Bit()); });
    });
    connect(preferencesDialog, &PreferencesDialog::subtitleAutoLoadChanged, this, [=](bool autoload)
    {
        if (subtitle->autoLoad() != autoload)
            QtConcurrent::run([=]{ subtitle->setAutoLoad(autoload); });
    });
    connect(preferencesDialog, &PreferencesDialog::subtitleEnabled, this, [=](bool enabled)
    {
        if (subtitle->isEnabled() != enabled)
            QtConcurrent::run([=]{ subtitle->setEnabled(enabled); });
    });
    connect(preferencesDialog, &PreferencesDialog::skinChanged, this, [=](const QString &skin)
    {
        if (SkinsManager::getInstance()->currentSkinName() != skin)
            QtConcurrent::run([=]{ SkinsManager::getInstance()->setSkin(skin); });
    });
    connect(preferencesDialog, &PreferencesDialog::rendererChanged, this, [=](QtAV::VideoRendererId rendererId)
    {
        if ((rendererId != renderer->id()) || !renderer)
        {
            QtAV::VideoRenderer *newRenderer = QtAV::VideoRenderer::create(rendererId);
            setRenderer(newRenderer);
        }
    });
    connect(preferencesDialog, &PreferencesDialog::imageQualityChanged, this, qOverload<const QString &>(&MainWindow::setImageQuality));
    connect(player, &QtAV::AVPlayer::positionChanged, this, [=](qint64 pos)
    {
        emit preferencesDialog->updateVideoSlider(pos);
        emit preferencesDialog->setVideoPositionText(QTime(0, 0, 0).addMSecs(pos).toString(QStringLiteral("HH:mm:ss")));
    });
    connect(this, &MainWindow::showOptions, this, &MainWindow::showPreferencesDialog);
    connect(this, qOverload<const QString &>(&MainWindow::play), this, &MainWindow::urlChanged);
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
    QtConcurrent::run([=]
    {
        subtitle->uninstall();
        player->setRenderer(videoRenderer);
    });
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
        QtConcurrent::run([=]{ renderer->forcePreferredPixelFormat(true); });
    else
        QtConcurrent::run([=]{ renderer->forcePreferredPixelFormat(false); });
    QtConcurrent::run([=]{ subtitle->installTo(renderer); });
    return true;
}

void MainWindow::setImageQuality(const QString &quality)
{
    if (!renderer)
        return;
    if ((quality == QStringLiteral("default")) &&
            (renderer->quality() != QtAV::VideoRenderer::QualityDefault))
        QtConcurrent::run([=]{ renderer->setQuality(QtAV::VideoRenderer::QualityDefault); });
    else if ((quality == QStringLiteral("best")) &&
             (renderer->quality() != QtAV::VideoRenderer::QualityBest))
        QtConcurrent::run([=]{ renderer->setQuality(QtAV::VideoRenderer::QualityBest); });
    else if ((quality == QStringLiteral("fastest")) &&
             (renderer->quality() != QtAV::VideoRenderer::QualityFastest))
        QtConcurrent::run([=]{ renderer->setQuality(QtAV::VideoRenderer::QualityFastest); });
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
        QtConcurrent::run([=]{ renderer->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio); });
    else if (!fit && (renderer->outAspectRatioMode() != QtAV::VideoRenderer::VideoAspectRatio))
        QtConcurrent::run([=]{ renderer->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio); });
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
    emit preferencesDialog->clearAllTracks();
    emit preferencesDialog->updateVideoSliderUnit(player->notifyInterval());
    emit preferencesDialog->updateVideoSliderRange(player->duration());
    emit preferencesDialog->updateVideoSlider(player->position());
    emit preferencesDialog->setSeekAreaEnabled(player->isSeekable());
    emit preferencesDialog->setAudioAreaEnabled(player->audio());
    emit preferencesDialog->updateVideoTracks(player->internalVideoTracks());
    emit preferencesDialog->updateAudioTracks(player->internalAudioTracks(), false);
    emit preferencesDialog->setVideoDurationText(QTime(0, 0, 0).addMSecs(player->mediaStopPosition()).toString(QStringLiteral("HH:mm:ss")));
    if (SettingsManager::getInstance()->getAudioAutoLoad())
        emit preferencesDialog->updateAudioTracks(player->externalAudioTracks(), true);
    emit preferencesDialog->updateSubtitleTracks(player->internalSubtitleTracks(), false);
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
            emit preferencesDialog->updateSubtitleTracks(externalSubtitleTracks, true);
        }
    }
    QtConcurrent::run([=]{ subtitle->setEnabled(false); });
    if (SettingsManager::getInstance()->getSubtitle())
    {
        if (player->subtitleStreamCount() > 0)
            QtConcurrent::run([=]
            {
                subtitle->setEnabled(true);
                player->setSubtitleStream(0);
            });
        else if (SettingsManager::getInstance()->getSubtitleAutoLoad())
        {
            QStringList externalSubtitles = Utils::externalFilesToLoad(QFileInfo(player->file()), QStringLiteral("sub"));
            if (!externalSubtitles.isEmpty())
            {
                QtConcurrent::run([=]{ subtitle->setEnabled(true); });
                if (subtitle->file() != externalSubtitles.constFirst())
                    QtConcurrent::run([=]{ subtitle->setFile(externalSubtitles.constFirst()); });
            }
        }
    }
}

void MainWindow::play()
{
    if (!player)
        return;
    if (player->isPaused())
        QtConcurrent::run([=]{ player->pause(false); });
}

void MainWindow::pause()
{
    if (!player)
        return;
    if (player->isPlaying())
        QtConcurrent::run([=]{ player->pause(); });
}

void MainWindow::stop()
{
    if (!player)
        return;
    if (player->isLoaded())
        QtConcurrent::run([=]{ player->stop(); });
}

void MainWindow::urlChanged(const QString &url)
{
    if (!player)
        return;
    if (SettingsManager::getInstance()->getHwdec())
    {
        QStringList decoders = SettingsManager::getInstance()->getDecoders();
        if (!decoders.contains(QStringLiteral("FFmpeg")))
            decoders << QStringLiteral("FFmpeg");
        if (player->videoDecoderPriority() != decoders)
            QtConcurrent::run([=]{ player->setVideoDecoderPriority(decoders); });
        if (decoders.contains(QStringLiteral("CUDA")))
        {
            QVariantHash cuda_opt;
            cuda_opt[QStringLiteral("surfaces")] = 0;
            cuda_opt[QStringLiteral("copyMode")] = QStringLiteral("ZeroCopy");
            QVariantHash opt;
            opt[QStringLiteral("CUDA")] = cuda_opt;
            QtConcurrent::run([=]{ player->setOptionsForVideoCodec(opt); });
        }
        if (decoders.contains(QStringLiteral("D3D11")))
        {
            QVariantHash d3d11_opt;
            //d3d11_opt[QStringLiteral("???")] = ???;
            d3d11_opt[QStringLiteral("copyMode")] = QStringLiteral("ZeroCopy");
            QVariantHash opt;
            opt[QStringLiteral("D3D11")] = d3d11_opt;
            QtConcurrent::run([=]{ player->setOptionsForVideoCodec(opt); });
        }
        if (decoders.contains(QStringLiteral("DXVA")))
        {
            QVariantHash dxva_opt;
            //dxva_opt[QStringLiteral("???")] = ???;
            dxva_opt[QStringLiteral("copyMode")] = QStringLiteral("ZeroCopy");
            QVariantHash opt;
            opt[QStringLiteral("DXVA")] = dxva_opt;
            QtConcurrent::run([=]{ player->setOptionsForVideoCodec(opt); });
        }
    }
    else if (player->videoDecoderPriority() != (QStringList() << QStringLiteral("FFmpeg")))
        QtConcurrent::run([=]{ player->setVideoDecoderPriority(QStringList() << QStringLiteral("FFmpeg")); });
    if (isHidden())
        show();
    if (!url.isEmpty())
    {
        QtConcurrent::run([=]{ player->play(url); });
        setWindowTitle(QFileInfo(url).fileName());
    }
    else
        play();
    emit preferencesDialog->volumeChanged(SettingsManager::getInstance()->getVolume());
}
