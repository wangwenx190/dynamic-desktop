#pragma once

#include <QSettings>

class SettingsManager
{
public:
    static SettingsManager *getInstance();

public:
    bool regAutostart();
    void unregAutostart();
    bool isRegAutostart() const;

    QString getUrl() const;
    bool getMute() const;
    unsigned int getVolume() const;
    bool getAutostart() const;
    bool getHwdec() const;

    void setUrl(const QString &url);
    void setMute(bool mute);
    void setVolume(unsigned int volume);
    void setAutostart(bool enable);
    void setHwdec(bool enable);

private:
    SettingsManager();
    ~SettingsManager();

private:
    QSettings *settings = nullptr;
};
