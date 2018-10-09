#pragma once

#include "framelesswindow.h"

#include <QtAV>
#include <QtAVWidgets>
#include <QWinTaskbarProgress>
#include <QWinTaskbarButton>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public FramelessWindow
{
    Q_OBJECT

signals:
    void pause();
    void muteChanged(bool);
    void volumeChanged(unsigned int);
    void urlChanged(const QString &);
    void autostartChanged(bool);
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

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override;

protected:
    void changeEvent(QEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void refreshUI();
    void setDecoders();
    void setRatio();

private:
    Ui::PreferencesDialog *ui;
    bool audioAvailable = true;
    unsigned int sliderUnit = 1000;
    QWinTaskbarButton *taskbarButton = nullptr;
    QWinTaskbarProgress *taskbarProgress = nullptr;
};
