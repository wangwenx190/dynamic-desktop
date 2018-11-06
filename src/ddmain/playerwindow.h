#pragma once

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QVBoxLayout)

namespace QtAV
{
    QT_FORWARD_DECLARE_CLASS(AVPlayer)
    QT_FORWARD_DECLARE_CLASS(SubtitleFilter)
    QT_FORWARD_DECLARE_CLASS(VideoRenderer)
}

class PlayerWindow : public QWidget
{
    Q_OBJECT

signals:
    void playStateChanged(bool);
    void volumeToolTipChanged(const QString &);
    void mediaPositionChanged(qint64);
    void videoPositionTextChanged(const QString &);
    void audioAreaEnableChanged(bool);
    void clearAllTracks();
    void mediaSliderUnitChanged(quint32);
    void mediaSliderRangeChanged(qint64);
    void seekAreaEnableChanged(bool);
    void videoTracksChanged(const QVariantList &);
    void audioTracksChanged(const QVariantList &, bool);
    void videoDurationTextChanged(const QString &);
    void subtitleTracksChanged(const QVariantList &, bool);

public:
    explicit PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow() override;

public slots:
    void setVolume(quint32 volume = 9);
    void setMute(bool mute = true);
    void seek(qint64 value = 0);
    void setVideoTrack(quint32 id = 0);
    void setAudioTrack(quint32 id = 0);
    void setSubtitleTrack(const QVariant& param);
    void setSubtitle(const QString &subPath);
    void setAudio(const QString &audioPath);
    void setCharset(const QString &charset = QStringLiteral("AutoDetect"));
    void setSubtitleAutoLoad(bool autoload = true);
    void setSubtitleEnabled(bool enabled = true);
    void play();
    void pause();
    void stop();
    void setUrl(const QString &url);
    bool setRenderer(int id);
    void setImageQuality(const QString &quality = QStringLiteral("best"));
    void setImageRatio(bool fit = true);
    void setWindowMode(bool enabled = true);

private slots:
    void initUI();
    void initConnections();
    void initPlayer();
    void initAudio();
    void onStartPlay();

private:
    QtAV::AVPlayer *player = nullptr;
    QtAV::VideoRenderer *renderer = nullptr;
    QtAV::SubtitleFilter *subtitle = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    bool windowMode = false;
};
