#pragma once

#include <QWidget>
#include <QtAV>
#include <QtAVWidgets>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_END_NAMESPACE

QT_FORWARD_DECLARE_CLASS(PreferencesDialog)

namespace QtAV
{
    QT_FORWARD_DECLARE_CLASS(AVPlayer)
    QT_FORWARD_DECLARE_CLASS(SubtitleFilter)
    QT_FORWARD_DECLARE_CLASS(VideoRenderer)
}

class PlayerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow() override;

public slots:
    void initConnections();
    void setPreferencesDialog(PreferencesDialog *preferences);
    void setVolume(quint32 volume);
    void setMute(bool mute = true);
    void seek(qint64 value);
    void setVideoTrack(quint32 id);
    void setAudioTrack(quint32 id);
    void setSubtitleTrack(const QVariant& param);
    void setSubtitle(const QString &subPath);
    void setAudio(const QString &audioPath);
    void setCharset(const QString &charset);
    void setSubtitleAutoLoad(bool autoload = true);
    void setSubtitleEnabled(bool enabled = true);
    void play();
    void pause();
    //void stop();
    void setUrl(const QString &url);
    bool setRenderer(QtAV::VideoRendererId id);
    void setImageQuality(const QString &quality);
    void setImageRatio(bool fit);

private slots:
    void initUI();
    void initPlayer();
    void initAudio();
    void onStartPlay();

private:
    QtAV::AVPlayer *player = nullptr;
    QtAV::VideoRenderer *renderer = nullptr;
    QtAV::SubtitleFilter *subtitle = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    PreferencesDialog *preferencesDialog = nullptr;
};
