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

    bool hasNvidiaCard() const;
    bool hasAmdCard() const;
    bool hasIntelCard() const;

    QStringList defaultDecoders() const;

    QString getUrl() const;
    bool getMute() const;
    unsigned int getVolume() const;
    bool getAutostart() const;
    bool getHwdec() const;
    QStringList getDecoders() const;

    void setUrl(const QString &url);
    void setMute(bool mute);
    void setVolume(unsigned int volume);
    void setAutostart(bool enable);
    void setHwdec(bool enable);
    void setDecoders(const QStringList &decoders);

private:
    SettingsManager();
    ~SettingsManager();

private:
    QSettings *settings = nullptr;
};
