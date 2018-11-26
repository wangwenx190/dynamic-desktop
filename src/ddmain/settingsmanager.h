#pragma once

#include <QSettings>

class SettingsManager
{
public:
    enum PlaybackMode
    {
        RepeatCurrentFile,
        RepeatCurrentPlaylist,
        RepeatAllPlaylists,
        RandomFileFromCurrentPlaylist,
        RandomFileFromAllPlaylists,
        RandomPlaylist
    };
    static SettingsManager *getInstance();

public:
    void clearPlaylist(const QString &name);

    QStringList getDefaultDecoders() const;
#ifndef DD_NO_MIME_TYPE
    QStringList getSupportedMimeTypes() const;
#endif
    QString getLastDir() const;

    QString getLastFile() const;
    bool getMute() const;
    quint32 getVolume() const;
    bool getHwdec() const;
    QStringList getDecoders() const;
    bool getFitDesktop() const;
    bool getSubtitle() const;
    QString getCharset() const;
    bool getSubtitleAutoLoad() const;
    bool getAudioAutoLoad() const;
#ifndef DD_NO_CSS
    QString getSkin() const;
#endif
#ifndef DD_NO_TRANSLATIONS
    QString getLanguage() const;
#endif
    int getRenderer() const;
    QString getImageQuality() const;
    bool getAutoCheckUpdate() const;
    PlaybackMode getPlaybackMode() const;
    QString getCurrentPlaylistName() const;
    QStringList getAllFilesFromPlaylist(const QString &name) const;
    QStringList getAllPlaylistNames() const;

    void setLastFile(const QString &url);
    void setMute(bool mute = true);
    void setVolume(quint32 volume = 9);
    void setHwdec(bool enable = false);
    void setDecoders(const QStringList &decoders = QStringList{ QStringLiteral("FFmpeg") });
    void setFitDesktop(bool fit = true);
    void setSubtitle(bool show = true);
    void setCharset(const QString &charset = QStringLiteral("AutoDetect"));
    void setSubtitleAutoLoad(bool autoload = true);
    void setAudioAutoLoad(bool autoload = true);
#ifndef DD_NO_CSS
    void setSkin(const QString &skin = QStringLiteral("Default"));
#endif
#ifndef DD_NO_TRANSLATIONS
    void setLanguage(const QString &lang = QStringLiteral("auto"));
#endif
    void setRenderer(int vid);
    void setImageQuality(const QString &quality = QStringLiteral("best"));
    void setAutoCheckUpdate(bool enabled = true);
    void setPlaybackMode(PlaybackMode playbackMode = PlaybackMode::RepeatCurrentFile);
    void setCurrentPlaylistName(const QString &name);
    void setPlaylistFiles(const QString &name, const QStringList &files);
    void setAllPlaylistNames(const QStringList &names);

private:
    explicit SettingsManager();
    ~SettingsManager();

private:
    QSettings *settings = nullptr;

private:
    Q_DISABLE_COPY(SettingsManager)
};
