#include "mainwindow.h"
#include <SettingsManager>
#include <Utils>
#include <Wallpaper>

#include <QMessageBox>
#include <QVBoxLayout>
#include <QtAV>
#include <QtAVWidgets>
#include <QFileInfo>

const qreal kVolumeInterval = 0.04;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    initUI();
    initPlayer();
    initConnections();
    initAudio();
}

MainWindow::~MainWindow()
{
    delete subtitle;
    delete renderer;
    delete player;
    delete mainLayout;
}

void MainWindow::parseCommand(const QPair<QString, QVariant> &command)
{
    QString action = command.first;
    QByteArray byteArray = action.toLatin1();
    const char *name = byteArray.constData();
    QVariant param = command.second;
    QMetaObject::invokeMethod(this, name, Q_ARG(QVariant, param));
}

void MainWindow::setVolume(const QVariant& param)
{
    quint32 volume = param.toUInt();
    QtAV::AudioOutput *ao = player ? player->audio() : nullptr;
    if (ao)
    {
        qreal newVolume = static_cast<qreal>(volume) * kVolumeInterval;
        if (ao->volume() != newVolume)
            if (qAbs(static_cast<int>(ao->volume() / kVolumeInterval) - volume) >= static_cast<int>(0.1 / kVolumeInterval))
                ao->setVolume(newVolume);
        //emit this->sendCommand(qMakePair(QStringLiteral("setVolumeToolTip"), tr("Volume: %0").arg(QString::number(newVolume))));
    }
}

void MainWindow::setMute(const QVariant& param)
{
    bool mute = param.toBool();
    if (player->audio())
        if (player->audio()->isMute() != mute)
            player->audio()->setMute(mute);
}

void MainWindow::seek(const QVariant& param)
{
    qint64 value = param.toLongLong();
    if (player->isLoaded() && player->isSeekable())
        player->seek(value);
}

void MainWindow::setVideoTrack(const QVariant& param)
{
    quint32 id = param.toUInt();
    if (player->isLoaded())
        if (id != player->currentVideoStream())
            player->setVideoStream(id);
}

void MainWindow::setAudioTrack(const QVariant& param)
{
    quint32 id = param.toUInt();
    if (player->isLoaded())
        if (id != player->currentAudioStream())
            player->setAudioStream(id);
}

void MainWindow::setSubtitleTrack(const QVariant& param)
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
                player->setSubtitleStream(id);
        }
    }
}

void MainWindow::setSubtitle(const QVariant& param)
{
    const QString subPath = param.toString();
    if (player->isLoaded())
        if (subtitle->file() != subPath)
            subtitle->setFile(subPath);
}

void MainWindow::setAudio(const QVariant& param)
{
    const QString audioPath = param.toString();
    if (player->isLoaded() && player->audio())
        player->setExternalAudio(audioPath);
}

void MainWindow::setCharset(const QVariant& param)
{
    const QString charset = param.toString();
    if (SettingsManager::getInstance()->getSubtitle())
        if (subtitle->codec() != charset.toLatin1())
            subtitle->setCodec(charset.toLatin1());
}

void MainWindow::setSubtitleAutoLoad(const QVariant& param)
{
    bool autoload = param.toBool();
    if (subtitle->autoLoad() != autoload)
        subtitle->setAutoLoad(autoload);
}

void MainWindow::setSubtitleEnabled(const QVariant& param)
{
    bool enabled = param.toBool();
    if (subtitle->isEnabled() != enabled)
        subtitle->setEnabled(enabled);
}

void MainWindow::initUI()
{
    mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    setWindowTitle(QStringLiteral("Dynamic Desktop"));
}

void MainWindow::initPlayer()
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

void MainWindow::initConnections()
{
    connect(player, &QtAV::AVPlayer::loaded, this, &MainWindow::onStartPlay);
    connect(player, &QtAV::AVPlayer::positionChanged, this, [=](qint64 pos)
    {
        emit this->sendCommand(qMakePair(QStringLiteral("updateVideoSlider"), pos));
        emit this->sendCommand(qMakePair(QStringLiteral("setVideoPositionText"), QTime(0, 0, 0).addMSecs(pos).toString(QStringLiteral("HH:mm:ss"))));
    });
}

void MainWindow::initAudio()
{
    if (player->audio())
    {
        setVolume(SettingsManager::getInstance()->getVolume());
        setMute(SettingsManager::getInstance()->getMute());
    }
    else
        emit this->sendCommand(qMakePair(QStringLiteral("setAudioAreaEnabled"), false));
}

bool MainWindow::setRenderer(const QVariant& param)
{
    if (!player || !subtitle)
        return false;
    auto id = static_cast<QtAV::VideoRendererId>(param.toInt());
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

void MainWindow::setImageQuality(const QVariant& param)
{
    if (!renderer)
        return;
    const QString quality = param.toString().toLower();
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

void MainWindow::setImageRatio(const QVariant& param)
{
    if (!renderer)
        return;
    bool fit = param.toBool();
    if (fit && (renderer->outAspectRatioMode() != QtAV::VideoRenderer::RendererAspectRatio))
        renderer->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
    else if (!fit && (renderer->outAspectRatioMode() != QtAV::VideoRenderer::VideoAspectRatio))
        renderer->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
}

void MainWindow::setTranslation(const QVariant &param)
{
    Utils::installTranslation(param.toString(), QStringLiteral("ply"));
}

void MainWindow::onStartPlay()
{
    if (!player || !subtitle)
        return;
    emit this->sendCommand(qMakePair(QStringLiteral("clearAllTracks"), QVariant()));
    emit this->sendCommand(qMakePair(QStringLiteral("updateVideoSliderUnit"), player->notifyInterval()));
    emit this->sendCommand(qMakePair(QStringLiteral("updateVideoSliderRange"), player->duration()));
    emit this->sendCommand(qMakePair(QStringLiteral("updateVideoSlider"), player->position()));
    emit this->sendCommand(qMakePair(QStringLiteral("setSeekAreaEnabled"), player->isSeekable()));
    emit this->sendCommand(qMakePair(QStringLiteral("setAudioAreaEnabled"), player->audio() ? true : false));
    emit this->sendCommand(qMakePair(QStringLiteral("updateVideoTracks"), player->internalVideoTracks()));
    emit this->sendCommand(qMakePair(QStringLiteral("updateAudioTracks"), player->internalAudioTracks()));
    emit this->sendCommand(qMakePair(QStringLiteral("setVideoDurationText"), QTime(0, 0, 0).addMSecs(player->mediaStopPosition()).toString(QStringLiteral("HH:mm:ss"))));
    //if (SettingsManager::getInstance()->getAudioAutoLoad())
    //emit this->sendCommand(qMakePair(QStringLiteral("updateAudioTracks"), qMakePair(player->externalAudioTracks(), true)));
    bool externalAudioLoaded = false;
    if (SettingsManager::getInstance()->getAudioAutoLoad())
    {
        QVariantList externalAudioTracks = player->externalAudioTracks();
        if (!externalAudioTracks.isEmpty())
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
    if (!externalAudioLoaded)
        player->setExternalAudio(QString());
    emit this->sendCommand(qMakePair(QStringLiteral("updateSubtitleTracks"), player->internalSubtitleTracks()));
    /*if (SettingsManager::getInstance()->getSubtitleAutoLoad())
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
            emit this->sendCommand(qMakePair(QStringLiteral("updateSubtitleTracks"), qMakePair(externalSubtitleTracks, true)));
        }
    }*/
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

void MainWindow::play(const QVariant& param)
{
    Q_UNUSED(param)
    if (!player)
        return;
    if (Utils::isPicture(player->file()))
        return;
    if (player->isPaused())
        player->pause(false);
}

void MainWindow::pause(const QVariant& param)
{
    Q_UNUSED(param)
    if (!player)
        return;
    if (player->isPlaying())
        player->pause();
}

/*void MainWindow::stop(const QVariant& param)
{
    Q_UNUSED(param)
    if (!player)
        return;
    if (player->isLoaded())
        player->stop();
}*/

void MainWindow::setUrl(const QVariant& param)
{
    if (!player)
        return;
    const QString url = param.toString();
    if (!url.isEmpty())
    {
        if (url == player->file())
        {
            if (!Utils::isPicture(url))
                play(QVariant());
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
        play(QVariant());
    if (!player->file().isEmpty() && (Utils::isVideo(player->file()) || Utils::isPicture(player->file())))
    {
        if (Wallpaper::isWallpaperHidden())
            Wallpaper::showWallpaper();
        if (isHidden())
            show();
    }
    else if (isVisible())
    {
        hide();
        if (Wallpaper::isWallpaperVisible())
            Wallpaper::hideWallpaper();
    }
}
