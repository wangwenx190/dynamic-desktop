#pragma once

#include <QtNiceFramelessWindow>

#ifndef DD_NO_WIN_EXTRAS
QT_FORWARD_DECLARE_CLASS(QWinTaskbarButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarProgress)
#endif
QT_FORWARD_DECLARE_CLASS(QComboBox)

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
#ifndef DD_NO_TRANSLATIONS
    void languageChanged(const QString &);
#endif
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
    void showPlaylistDialog();
    void playlistChanged(const QString &);
    void playbackModeChanged(quint32);
    void repeatCurrentFile(bool);
#ifndef DD_NO_CSS
    void skinChanged(const QString &);
#endif

public slots:
#ifndef DD_NO_TRANSLATIONS
    void refreshTexts(const QString &language);
#endif
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
    void playNextMedia();
    void playNextPlaylist();
    void playPreviousMedia();
    void playPreviousPlaylist();
    void refreshPlaylistsAndFiles();
    void switchPlaylist(const QString &name);
    void switchFile(const QString &url);
    void mediaEndReached();
    void playRandomFileFromAllPlaylistsFiles();

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override;

protected:
#if !defined(DD_NO_DRAG_DROP) && !defined(DD_NO_MIME_TYPE)
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
#endif
    void closeEvent(QCloseEvent *event) override;

private slots:
    void initUI();
    void initConnections();
    void setDecoders();
    void setRatio();
#ifndef DD_NO_CSS
    void populateSkins(const QString &dirPath, bool add = true, bool isExternal = false);
#endif
#ifndef DD_NO_TRANSLATIONS
    void populateLanguages(const QString &dirPath, bool add = true, bool isExternal = false);
#endif
    void initIcons();
    void populateFiles();
    void populatePlaylists();
    void moveNextItem(QComboBox *comboBox);
    void movePreviousItem(QComboBox *comboBox);
    void switchRandomItem(QComboBox *comboBox);
    void switchToItem(QComboBox *comboBox, const QString &text);
    void switchToRandomFile();
    void switchToRandomPlaylist();
    int getRandomNumber(int max);

private:
    Ui::PreferencesDialog *ui = nullptr;
    bool audioAvailable = true, isPlaying = false, refreshingData = false;
    quint32 sliderUnit = 1000;
#ifndef DD_NO_WIN_EXTRAS
    QWinTaskbarButton *taskbarButton = nullptr;
    QWinTaskbarProgress *taskbarProgress = nullptr;
#endif

private:
    Q_DISABLE_COPY(PreferencesDialog)
};
