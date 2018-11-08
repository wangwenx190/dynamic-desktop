#pragma once

#include <QtNiceFramelessWindow>

QT_FORWARD_DECLARE_CLASS(QWinTaskbarButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarProgress)

namespace Ui
{
    class PreferencesDialog;
}

class PreferencesDialog : public CFramelessWindow
{
    Q_OBJECT

signals:
    void muteChanged(bool);
    void about();
    void play();
    void pause();
    void languageChanged(const QString &);
    void urlChanged(const QString &);
    void audioFileChanged(const QString &);
    void subtitleFileChanged(const QString &);
    void volumeChanged(quint32);
    void seek(qint64);
    void videoTrackChanged(quint32);
    void audioTrackChanged(quint32);
    void subtitleTrackChanged(const QVariant &);
    void rendererChanged(int);
    void imageQualityChanged(const QString &);
    void charsetChanged(const QString &);
    void subtitleAutoLoadChanged(bool);
    void subtitleEnableChanged(bool);
    void imageRatioChanged(bool);

public slots:
    void refreshTexts(const QString &language);
    void setMute(bool mute = true);
    void setPlaying(bool playing = true);
    void togglePlayPause();
    void clearAllTracks();
    void setMediaSliderPosition(qint64 position = 0);
    void setMediaSliderUnit(quint32 unit = 1000);
    void setMediaSliderRange(qint64 duration);
    void setSeekAreaEnabled(bool enabled = true);
    void setAudioAreaEnabled(bool available = true);
    void setVideoTracks(const QVariantList &videoTracks);
    void setAudioTracks(const QVariantList &audioTracks, bool add = false);
    void setSubtitleTracks(const QVariantList &subtitleTracks, bool add = false);
    void setVideoPositionText(const QString &text);
    void setVideoDurationText(const QString &text);

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void initUI();
    void initConnections();
    void setDecoders();
    void setRatio();
    void populateSkins(const QString &dirPath, bool add = true, bool isExternal = false);
    void populateLanguages(const QString &dirPath, bool add = true, bool isExternal = false);

private:
    Ui::PreferencesDialog *ui = nullptr;
    bool audioAvailable = true, isPlaying = false;
    quint32 sliderUnit = 1000;
    QWinTaskbarButton *taskbarButton = nullptr;
    QWinTaskbarProgress *taskbarProgress = nullptr;
};
