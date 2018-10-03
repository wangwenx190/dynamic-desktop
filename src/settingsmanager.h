#pragma once

#include <QSettings>
#include <QtAV>
#include <QtAVWidgets>

class SettingsManager
{
public:
    static SettingsManager *getInstance();

public:
    bool regAutostart();
    void unregAutostart();

    QStringList defaultDecoders() const;
    QStringList supportedMimeTypes() const;
    QString lastDir() const;

    QString getUrl() const;
    bool getMute() const;
    unsigned int getVolume() const;
    bool getAutostart() const;
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

    void setUrl(const QString &url);
    void setMute(bool mute);
    void setVolume(unsigned int volume);
    void setAutostart(bool enable);
    void setHwdec(bool enable);
    void setDecoders(const QStringList &decoders);
    void setFitDesktop(bool fit);
    void setSubtitle(bool show);
    void setCharset(const QString &charset);
    void setSubtitleAutoLoad(bool autoload);
    void setAudioAutoLoad(bool autoload);
    void setSkin(const QString &skin);
    void setLanguage(const QString &lang);
    void setRenderer(QtAV::VideoRendererId vid);

private:
    SettingsManager();
    ~SettingsManager();

private:
    QSettings *settings;
};
