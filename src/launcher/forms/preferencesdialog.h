#pragma once

#include <QtNiceFramelessWindow>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QWinTaskbarButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarProgress)
QT_END_NAMESPACE

#include <QtAV>
#include <QtAVWidgets>

namespace Ui
{
    class PreferencesDialog;
}

class PreferencesDialog : public CFramelessWindow
{
    Q_OBJECT

signals:
    void pause();
    void muteChanged(bool);
    void volumeChanged(unsigned int);
    void urlChanged(const QString &);
    void about();
    void seekBySlider(qint64);
    void pictureRatioChanged(bool);
    void videoTrackChanged(unsigned int);
    void audioTrackChanged(unsigned int);
    void subtitleTrackChanged(const QVariant &);
    void charsetChanged(const QString &);
    void subtitleAutoLoadChanged(bool);
    void subtitleEnabled(bool);
    void subtitleOpened(const QString &);
    void audioOpened(const QString &);
    void skinChanged(const QString &);
    void rendererChanged(QtAV::VideoRendererId);
    void imageQualityChanged(const QString &);
    //void requestUpdate();

signals:
    void updateVideoSlider(qint64);
    void updateVideoSliderUnit(int);
    void updateVideoSliderRange(qint64);
    void setSeekAreaEnabled(bool);
    void setAudioAreaEnabled(bool);
    void setVolumeAreaEnabled(bool);
    void updateVolumeArea();
    void updateVideoTracks(const QVariantList &);
    void updateAudioTracks(const QVariantList &, bool);
    void updateSubtitleTracks(const QVariantList &, bool);
    void clearAllTracks();
    void setVolumeToolTip(const QString &);
    void setVideoPositionText(const QString &);
    void setVideoDurationText(const QString &);

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override;

public:
    bool setAutoStart(bool enable = true);
    bool isAutoStart(const QString &name = QStringLiteral("Dynamic Desktop Service"));

protected:
    void changeEvent(QEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void initUI();
    void initConnections();
    void setDecoders();
    void setRatio();

private:
    Ui::PreferencesDialog *ui;
    bool audioAvailable = true;
    unsigned int sliderUnit = 1000;
    QWinTaskbarButton *taskbarButton = nullptr;
    QWinTaskbarProgress *taskbarProgress = nullptr;
};
