#pragma once

#include <QSettings>
#include <QtAV>
#include <QtAVWidgets>

class SettingsManager
{
public:
    static SettingsManager *getInstance();

public:
    bool setAutoStart(bool enable = true);
    bool isAutoStart();

    QStringList defaultDecoders() const;
    QStringList supportedMimeTypes() const;
    QString lastDir() const;

    QString getUrl() const;
    bool getMute() const;
    unsigned int getVolume() const;
    bool getHwdec() const;
    QStringList getDecoders() const;
    bool getFitDesktop() const;
    bool getSubtitle() const;
    QString getCharset() const;
    bool getSubtitleAutoLoad() const;
    bool getAudioAutoLoad() const;
    QString getSkin() const;
    QString getLanguage() const;
    QtAV::VideoRendererId getRenderer() const;
    QString getImageQuality() const;

    void setUrl(const QString &url);
    void setMute(bool mute = true);
    void setVolume(unsigned int volume = 9);
    void setHwdec(bool enable = false);
    void setDecoders(const QStringList &decoders = QStringList{ QStringLiteral("FFmpeg") });
    void setFitDesktop(bool fit = true);
    void setSubtitle(bool show = true);
    void setCharset(const QString &charset = QStringLiteral("AutoDetect"));
    void setSubtitleAutoLoad(bool autoload = true);
    void setAudioAutoLoad(bool autoload = true);
    void setSkin(const QString &skin = QStringLiteral("Default"));
    void setLanguage(const QString &lang = QStringLiteral("auto"));
    void setRenderer(QtAV::VideoRendererId vid = QtAV::VideoRendererId_GLWidget2);
    void setImageQuality(const QString &quality = QStringLiteral("best"));

private:
    SettingsManager();
    ~SettingsManager();

private:
    QSettings *settings;
};
