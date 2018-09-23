#pragma once

#include <QSettings>

class SettingsManager
{
public:
    static SettingsManager *getInstance();

public:
    bool regAutostart();
    void unregAutostart();
    bool isRegAutostart();

    QString getUrl() const;
    bool getMute() const;
    unsigned int getVolume() const;
    bool getAutostart() const;

    void setUrl(const QString &url);
    void setMute(bool mute);
    void setVolume(unsigned int volume);
    void setAutostart(bool enable);

private:
    SettingsManager();
    ~SettingsManager();

private:
    QSettings *settings = nullptr;
};
