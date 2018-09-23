#include "settingsmanager.h"

#include <QCoreApplication>
#include <QDir>

SettingsManager *SettingsManager::getInstance()
{
    static SettingsManager settingsManager;
    return &settingsManager;
}

bool SettingsManager::regAutostart()
{
    const QString key = QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    QSettings set(key, QSettings::NativeFormat);
    if (set.status() != QSettings::NoError)
    {
        return false;
    }
    QString value = QLatin1Char('"') + QCoreApplication::applicationFilePath() + QLatin1Char('"');
    set.setValue(QStringLiteral("Dynamic Desktop"), QDir::toNativeSeparators(value));
    return true;
}

void SettingsManager::unregAutostart()
{
    const QString key = QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    QSettings set(key, QSettings::NativeFormat);
    if (set.contains(QStringLiteral("Dynamic Desktop")))
        set.remove(QStringLiteral("Dynamic Desktop"));
}

bool SettingsManager::isRegAutostart() const
{
    const QString key = QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    QSettings set(key, QSettings::NativeFormat);
    return set.contains(QStringLiteral("Dynamic Desktop"));
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
    return settings->value(QStringLiteral("volume"), 9).toUInt();
}

bool SettingsManager::getAutostart() const
{
    return settings->value(QStringLiteral("autostart"), false).toBool();
}

bool SettingsManager::getHwdec() const
{
    return settings->value(QStringLiteral("hwdec"), true).toBool();
}

QStringList SettingsManager::getDecoders() const
{
    return settings->value(QStringLiteral("decoders"), QStringList()
                           << QStringLiteral("CUDA")
                           << QStringLiteral("D3D11")
                           << QStringLiteral("DXVA")
                           << QStringLiteral("FFmpeg")).toStringList();
}

void SettingsManager::setUrl(const QString &url)
{
    settings->setValue(QStringLiteral("url"), url);
}

void SettingsManager::setMute(bool mute)
{
    settings->setValue(QStringLiteral("mute"), mute);
}

void SettingsManager::setVolume(unsigned int volume)
{
    settings->setValue(QStringLiteral("volume"), volume);
}

void SettingsManager::setAutostart(bool enable)
{
    settings->setValue(QStringLiteral("autostart"), enable);
}

void SettingsManager::setHwdec(bool enable)
{
    settings->setValue(QStringLiteral("hwdec"), enable);
}

void SettingsManager::setDecoders(const QStringList &decoders)
{
    settings->setValue(QStringLiteral("decoders"), decoders);
}

SettingsManager::SettingsManager()
{
    QString iniPath = QCoreApplication::applicationFilePath();
    if (iniPath.endsWith(QStringLiteral(".exe"), Qt::CaseInsensitive))
        iniPath = iniPath.remove(iniPath.lastIndexOf(QLatin1Char('.')), 4);
    iniPath += QStringLiteral(".ini");
    settings = new QSettings(iniPath, QSettings::IniFormat);
}

SettingsManager::~SettingsManager()
{
    delete settings;
}
