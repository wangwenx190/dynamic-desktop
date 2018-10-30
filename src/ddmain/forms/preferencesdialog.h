#pragma once

#include "qtniceframelesswindow.h"

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QWinTaskbarButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarProgress)
QT_END_NAMESPACE

QT_FORWARD_DECLARE_CLASS(PlayerWindow)

namespace Ui
{
    class PreferencesDialog;
}

class PreferencesDialog : public CFramelessWindow
{
    Q_OBJECT

signals:
    void setMute(bool);
    void muteChanged(bool);
    void about();
    void languageChanged();

public slots:
    void initConnections();
    void setPlayerWindow(PlayerWindow *player);
    void updateVideoSlider(qint64 position);
    void updateVideoSliderUnit(quint32 unit);
    void updateVideoSliderRange(qint64 duration);
    void setSeekAreaEnabled(bool enabled);
    void setAudioAreaEnabled(bool available);
    void updateVideoTracks(const QVariantList &videoTracks);
    void updateAudioTracks(const QVariantList &audioTracks, bool add = false);
    void updateSubtitleTracks(const QVariantList &subtitleTracks, bool add = false);
    void clearAllTracks();
    void setVolumeToolTip(const QString &text);
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
    void setDecoders();
    void setRatio();

private:
    Ui::PreferencesDialog *ui;
    bool audioAvailable = true;
    quint32 sliderUnit = 1000;
    QWinTaskbarButton *taskbarButton = nullptr;
    QWinTaskbarProgress *taskbarProgress = nullptr;
    PlayerWindow *playerWindow = nullptr;
};
