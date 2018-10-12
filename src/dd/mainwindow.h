#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QSystemTrayIcon)
QT_END_NAMESPACE

namespace QtAV
{
    QT_FORWARD_DECLARE_CLASS(AVPlayer)
    QT_FORWARD_DECLARE_CLASS(SubtitleFilter)
    QT_FORWARD_DECLARE_CLASS(VideoRenderer)
}

QT_FORWARD_DECLARE_CLASS(AboutDialog)
QT_FORWARD_DECLARE_CLASS(PreferencesDialog)

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void showOptions();
    void play(const QString &);

private slots:
    void initUI();
    void initPlayer();
    void initConnections();
    void disconnectAll();
    void refreshPlayer();
    bool setRenderer(QtAV::VideoRenderer *videoRenderer);
    void setImageQuality(const QString &quality);
    void setImageQuality();
    void setImageRatio(bool fit);
    void setImageRatio();
    void showAboutDialog();
    void showPreferencesDialog();
    void onStartPlay();
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
    QAction *muteAction = nullptr;
};
