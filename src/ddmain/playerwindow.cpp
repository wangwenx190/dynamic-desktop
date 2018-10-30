#include "playerwindow.h"
#include "settingsmanager.h"
#include "utils.h"
#include "wallpaper.h"
#include "forms/preferencesdialog.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QFileInfo>

const qreal kVolumeInterval = 0.04;

PlayerWindow::PlayerWindow(QWidget *parent) : QWidget(parent)
{
    initUI();
    initPlayer();
}

PlayerWindow::~PlayerWindow()
{
    delete subtitle;
    delete renderer;
    delete player;
    delete mainLayout;
}

void PlayerWindow::setPreferencesDialog(PreferencesDialog *preferences)
{
    preferencesDialog = preferences;
}

void PlayerWindow::setVolume(quint32 volume)
{
    QtAV::AudioOutput *ao = player ? player->audio() : nullptr;
    if (ao)
    {
        qreal newVolume = static_cast<qreal>(volume) * kVolumeInterval;
        if (ao->volume() != newVolume)
            if (qAbs(static_cast<int>(ao->volume() / kVolumeInterval) - static_cast<int>(volume)) >= static_cast<int>(0.1 / kVolumeInterval))
                ao->setVolume(newVolume);
        preferencesDialog->setVolumeToolTip(tr("Volume: %0").arg(QString::number(newVolume)));
    }
}

void PlayerWindow::setMute(bool mute)
{
    if (player->audio())
        if (player->audio()->isMute() != mute)
            player->audio()->setMute(mute);
}

void PlayerWindow::seek(qint64 value)
{
    if (player->isLoaded() && player->isSeekable())
        player->seek(value);
}

void PlayerWindow::setVideoTrack(quint32 id)
{
    if (player->isLoaded())
        if (id != player->currentVideoStream())
            player->setVideoStream(id);
}

void PlayerWindow::setAudioTrack(quint32 id)
{
    if (player->isLoaded())
        if (id != player->currentAudioStream())
            player->setAudioStream(id);
}

void PlayerWindow::setSubtitleTrack(const QVariant& param)
{
    if (player->isLoaded())
    {
        const QString newSubFile = param.toString();
        if (QFileInfo::exists(newSubFile) && subtitle->file() != newSubFile)
            subtitle->setFile(newSubFile);
        else
        {
            quint32 id = param.toUInt();
            if (id != player->currentSubtitleStream())
            {
                if (!subtitle->file().isEmpty())
                    subtitle->setFile(QString());
                player->setSubtitleStream(id);
            }
        }
    }
}

void PlayerWindow::setSubtitle(const QString& subPath)
{
    if (player->isLoaded())
        if (subtitle->file() != subPath)
            subtitle->setFile(subPath);
}

void PlayerWindow::setAudio(const QString& audioPath)
{
    if (player->isLoaded() && player->audio())
        player->setExternalAudio(audioPath);
}

void PlayerWindow::setCharset(const QString& charset)
{
    if (SettingsManager::getInstance()->getSubtitle())
        if (subtitle->codec() != charset.toLatin1())
            subtitle->setCodec(charset.toLatin1());
}

void PlayerWindow::setSubtitleAutoLoad(bool autoload)
{
    if (subtitle->autoLoad() != autoload)
        subtitle->setAutoLoad(autoload);
}

void PlayerWindow::setSubtitleEnabled(bool enabled)
{
    if (subtitle->isEnabled() != enabled)
        subtitle->setEnabled(enabled);
}

void PlayerWindow::initUI()
{
    mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    setWindowTitle(QStringLiteral("Dynamic Desktop"));
}

void PlayerWindow::initPlayer()
{
    player = new QtAV::AVPlayer();
    player->setRepeat(-1);
    subtitle = new QtAV::SubtitleFilter();
    subtitle->setPlayer(player);
    subtitle->setCodec(SettingsManager::getInstance()->getCharset().toLatin1());
    subtitle->setEngines(QStringList() << QStringLiteral("LibASS") << QStringLiteral("FFmpeg"));
    subtitle->setAutoLoad(SettingsManager::getInstance()->getSubtitleAutoLoad());
    subtitle->setEnabled(SettingsManager::getInstance()->getSubtitle());
    setRenderer(SettingsManager::getInstance()->getRenderer());
    setImageQuality(SettingsManager::getInstance()->getImageQuality());
    setImageRatio(SettingsManager::getInstance()->getFitDesktop());
}

void PlayerWindow::initConnections()
{
    connect(player, &QtAV::AVPlayer::loaded, this, &PlayerWindow::onStartPlay);
    connect(player, &QtAV::AVPlayer::positionChanged, this, [=](qint64 pos)
    {
        preferencesDialog->updateVideoSlider(pos);
        preferencesDialog->setVideoPositionText(QTime(0, 0, 0).addMSecs(pos).toString(QStringLiteral("HH:mm:ss")));
    });
    initAudio();
}

void PlayerWindow::initAudio()
{
    if (player->audio())
    {
        setVolume(SettingsManager::getInstance()->getVolume());
        setMute(SettingsManager::getInstance()->getMute());
    }
    else
        preferencesDialog->setAudioAreaEnabled(false);
}

bool PlayerWindow::setRenderer(QtAV::VideoRendererId id)
{
    if (!player || !subtitle)
        return false;
    if ((renderer != nullptr) && (id == renderer->id()))
        return false;
    QtAV::VideoRenderer *videoRenderer = QtAV::VideoRenderer::create(id);
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

void PlayerWindow::setImageQuality(const QString& quality)
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

void PlayerWindow::setImageRatio(bool fit)
{
    if (!renderer)
        return;
    if (fit && (renderer->outAspectRatioMode() != QtAV::VideoRenderer::RendererAspectRatio))
        renderer->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
    else if (!fit && (renderer->outAspectRatioMode() != QtAV::VideoRenderer::VideoAspectRatio))
        renderer->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
}

void PlayerWindow::setWindowMode(bool enabled)
{
    if (windowMode != enabled)
        windowMode = enabled;
}

void PlayerWindow::onStartPlay()
{
    if (!player || !subtitle)
        return;
    preferencesDialog->clearAllTracks();
    preferencesDialog->updateVideoSliderUnit(player->notifyInterval());
    preferencesDialog->updateVideoSliderRange(player->duration());
    preferencesDialog->updateVideoSlider(player->position());
    preferencesDialog->setSeekAreaEnabled(player->isSeekable());
    preferencesDialog->setAudioAreaEnabled(player->audio() ? true : false);
    preferencesDialog->updateVideoTracks(player->internalVideoTracks());
    preferencesDialog->updateAudioTracks(player->internalAudioTracks(), false);
    preferencesDialog->setVideoDurationText(QTime(0, 0, 0).addMSecs(player->mediaStopPosition()).toString(QStringLiteral("HH:mm:ss")));
    bool externalAudioLoaded = false;
    if (SettingsManager::getInstance()->getAudioAutoLoad())
    {
        QVariantList externalAudioTracks = player->externalAudioTracks();
        if (!externalAudioTracks.isEmpty())
        {
            preferencesDialog->updateAudioTracks(player->externalAudioTracks(), true);
            for (auto& track : externalAudioTracks)
            {
                QVariantMap trackData = track.toMap();
                const QString audioFilePath = trackData[QStringLiteral("file")].toString();
                if (!audioFilePath.isEmpty())
                    if (player->setExternalAudio(audioFilePath))
                    {
                        externalAudioLoaded = true;
                        break;
                    }
                    else
                        continue;
            }
        }
    }
    if (!externalAudioLoaded)
        player->setExternalAudio(QString());
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
    if (!subtitle->file().isEmpty())
        subtitle->setFile(QString());
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
                subtitle->setFile(externalSubtitles.constFirst());
            }
        }
    }
}

void PlayerWindow::play()
{
    if (!player)
        return;
    if (Utils::isPicture(player->file()))
        return;
    if (player->isPaused())
        player->pause(false);
}

void PlayerWindow::pause()
{
    if (!player)
        return;
    if (player->isPlaying())
        player->pause();
}

/*void PlayerWindow::stop()
{
    if (!player)
        return;
    if (player->isLoaded())
        player->stop();
}*/

void PlayerWindow::setUrl(const QString& url)
{
    if (!player)
        return;
    if (!url.isEmpty())
    {
        if (url == player->file())
        {
            if (!Utils::isPicture(url))
                play();
            return;
        }
        player->stop();
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
        player->play(url);
        setWindowTitle(QFileInfo(url).fileName());
    }
    else if (!player->file().isEmpty() && !Utils::isPicture(player->file()))
        play();
    if (!player->file().isEmpty() && (Utils::isVideo(player->file()) || Utils::isPicture(player->file())))
    {
        if (!windowMode)
            if (Wallpaper::isWallpaperHidden())
                Wallpaper::showWallpaper();
        if (isHidden())
            show();
    }
    else if (isVisible())
    {
        hide();
        if (!windowMode)
            if (Wallpaper::isWallpaperVisible())
                Wallpaper::hideWallpaper();
    }
}
