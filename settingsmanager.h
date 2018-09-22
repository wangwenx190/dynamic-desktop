#pragma once

#include <QObject>
#include <QSettings>

class SettingsManager
{
    Q_OBJECT

signals:
    void urlChanged(const QString &url);
    void muteChanged(bool mute);
    void volumeChanged(unsigned int volume);

public:
    static SettingsManager *getInstance();

public slots:
    QString getUrl() const;
    bool getMute() const;
    unsigned int getVolume() const;

public slots:
    void setUrl(const QString &url);
    void setMute(bool mute);
    void setVolume(unsigned int volume);

private:
    SettingsManager();
    ~SettingsManager();

private:
    QSettings *settings = nullptr;
};
