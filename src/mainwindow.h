#pragma once

#include "forms/aboutdialog.h"
#include "forms/preferencesdialog.h"

#include <QWidget>
#include <QtAV>
#include <QtAVWidgets>
#include <QVBoxLayout>
#include <QMenu>
#include <QSystemTrayIcon>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void showOptions();
    void play(const QString &);

private slots:
    void init();
    bool setRenderer(QtAV::VideoRenderer *videoRenderer);
    void setImageQuality(const QString &quality);
    void setImageQuality();
    void setImageRatio(bool fit);
    void setImageRatio();
    void showAboutDialog();
    void showPreferencesDialog();
    void updateControlPanel();
    void play();
    void pause();
    void stop();
    void urlChanged(const QString &url);

private:
    QtAV::AVPlayer *player = nullptr;
    QtAV::VideoRenderer *renderer = nullptr;
    QtAV::SubtitleFilter *subtitle = nullptr;
    QMenu *trayMenu = nullptr;
    QSystemTrayIcon *trayIcon = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    AboutDialog *aboutDialog = nullptr;
    PreferencesDialog *preferencesDialog = nullptr;
};
