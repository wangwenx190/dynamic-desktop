#include "settingsmanager.h"

#include <QCoreApplication>
#include <QDir>

SettingsManager *SettingsManager::getInstance()
{
    static SettingsManager settingsManager;
    return &settingsManager;
}

QString SettingsManager::getUrl() const
{
    return settings->value(QStringLiteral("url"), QString()).toString();
}

bool SettingsManager::getMute() const
{
    return settings->value(QStringLiteral("mute"), false).toBool();
}

unsigned int SettingsManager::getVolume() const
{
    return settings->value(QStringLiteral("volume"), 10).toInt();
}

void SettingsManager::setUrl(const QString &url)
{
    settings->setValue(QStringLiteral("url"), url);
    emit urlChanged(url);
}

void SettingsManager::setMute(bool mute)
{
    settings->setValue(QStringLiteral("mute"), mute);
    emit muteChanged(mute);
}

void SettingsManager::setVolume(unsigned int volume)
{
    settings->setValue(QStringLiteral("volume"), volume);
    emit volumeChanged(volume);
}

SettingsManager::SettingsManager()
{
    settings = new QSettings(QCoreApplication::applicationDirPath() + QDir::separator() + QStringLiteral("config.ini"), QSettings::IniFormat);
}

SettingsManager::~SettingsManager()
{
    delete settings;
}
