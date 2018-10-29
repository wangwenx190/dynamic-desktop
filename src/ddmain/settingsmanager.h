#pragma once

#include <QSettings>
#include <QtAV>
#include <QtAVWidgets>

class SettingsManager
{
public:
    static SettingsManager *getInstance();

public:
    QStringList defaultDecoders() const;
    QStringList supportedMimeTypes() const;
    QString lastDir() const;

    QString getUrl() const;
    bool getMute() const;
    quint32 getVolume() const;
    bool getHwdec() const;
    QStringList getDecoders() const;
    bool getFitDesktop() const;
    bool getSubtitle() const;
    QString getCharset() const;
    bool getSubtitleAutoLoad() const;
    bool getAudioAutoLoad() const;
    QString getSkin() const;
    QString getLanguage() const;
    int getRenderer() const;
    QString getImageQuality() const;
    QStringList getHistory() const;
    bool isHistoryEnabled() const;
    quint32 getHistoryMax() const;
    bool getAutoCheckUpdate() const;

    void setUrl(const QString &url);
    void setMute(bool mute = true);
    void setVolume(quint32 volume = 9);
    void setHwdec(bool enable = false);
    void setDecoders(const QStringList &decoders = QStringList{ QStringLiteral("FFmpeg") });
    void setFitDesktop(bool fit = true);
    void setSubtitle(bool show = true);
    void setCharset(const QString &charset = QStringLiteral("AutoDetect"));
    void setSubtitleAutoLoad(bool autoload = true);
    void setAudioAutoLoad(bool autoload = true);
    void setSkin(const QString &skin = QStringLiteral("Default"));
    void setLanguage(const QString &lang = QStringLiteral("auto"));
    void setRenderer(int vid = static_cast<int>(QtAV::VideoRendererId_GLWidget2));
    void setImageQuality(const QString &quality = QStringLiteral("best"));
    void setHistory(const QStringList &history);
    void setHistoryEnabled(bool enabled = true);
    void setHistoryMax(int max = 20);
    void setAutoCheckUpdate(bool enabled = true);

private:
    SettingsManager();
    ~SettingsManager();

private:
    QSettings *settings;
};
