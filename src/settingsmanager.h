#pragma once

#include <QSettings>

class SettingsManager
{
public:
    static SettingsManager *getInstance();

public:
    bool regAutostart();
    void unregAutostart();

    QStringList defaultDecoders() const;
    QStringList supportedMimeTypes();
    QStringList supportedSuffixes();

    QString getUrl() const;
    bool getMute() const;
    unsigned int getVolume() const;
    bool getAutostart() const;
    bool getHwdec() const;
    QStringList getDecoders() const;
    bool getLocalize() const;
    bool getFitDesktop() const;

    void setUrl(const QString &url);
    void setMute(bool mute);
    void setVolume(unsigned int volume);
    void setAutostart(bool enable);
    void setHwdec(bool enable);
    void setDecoders(const QStringList &decoders);
    void setLocalize(bool enable);
    void setFitDesktop(bool fit);

private:
    SettingsManager();
    ~SettingsManager();

private:
    QSettings *settings;
};
