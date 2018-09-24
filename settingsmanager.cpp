#include "settingsmanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QUrl>
#include <QFileInfo>

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
        return false;
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

bool SettingsManager::hasNvidiaCard() const
{
    const QString key = QStringLiteral("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\nvlddmkm\\Device0");
    QSettings set(key, QSettings::NativeFormat);
    if (set.status() != QSettings::NoError)
        return false;
    QString description = set.value(QStringLiteral("Device Description")).toString();
    return description.contains(QStringLiteral("NVIDIA"), Qt::CaseInsensitive);
}

bool SettingsManager::hasAmdCard() const
{
    const QString key = QStringLiteral("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\amdkmdap\\Device0");
    QSettings set(key, QSettings::NativeFormat);
    if (set.status() != QSettings::NoError)
        return false;
    QString description = set.value(QStringLiteral("Device Description")).toString();
    return description.contains(QStringLiteral("AMD"), Qt::CaseInsensitive);
}

bool SettingsManager::hasIntelCard() const
{
    const QString key = QStringLiteral("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\igfx\\Device0");
    QSettings set(key, QSettings::NativeFormat);
    if (set.status() != QSettings::NoError)
        return false;
    QString description = set.value(QStringLiteral("Device Description")).toString();
    return description.contains(QStringLiteral("Intel"), Qt::CaseInsensitive);
}

QStringList SettingsManager::defaultDecoders() const
{
    QStringList decoders;
    if (hasNvidiaCard())
        decoders << QStringLiteral("CUDA");
    if (hasNvidiaCard() || hasAmdCard() || hasIntelCard())
        decoders << QStringLiteral("D3D11") << QStringLiteral("DXVA");
    decoders << QStringLiteral("FFmpeg");
    return decoders;
}

QString SettingsManager::getUrl() const
{
    QString path = settings->value(QStringLiteral("url"), QString()).toString();
    if (QFileInfo(path).isDir())
        return QString();
    if (!QFileInfo::exists(path))
    {
        QUrl url(path);
        if (!url.isValid())
            return QString();
        if (url.isLocalFile())
            return url.toLocalFile();
        return QUrl::fromPercentEncoding(url.toEncoded());
    }
    return path;
}

bool SettingsManager::getMute() const
{
    return settings->value(QStringLiteral("mute"), false).toBool();
}

unsigned int SettingsManager::getVolume() const
{
    int vol = settings->value(QStringLiteral("volume"), 9).toInt();
    if (vol < 0)
        vol = 0;
    if (vol > 99)
        vol = 99;
    return static_cast<unsigned int>(vol);
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
    QStringList decoders = settings->value(QStringLiteral("decoders"), defaultDecoders()).toStringList();
    for (auto& decoder : decoders)
    {
        if (decoder != QStringLiteral("CUDA")
                && decoder != QStringLiteral("D3D11")
                && decoder != QStringLiteral("DXVA")
                && decoder != QStringLiteral("FFmpeg"))
        {
            decoder.clear();
            continue;
        }
        if (decoder == QStringLiteral("CUDA") && !hasNvidiaCard())
        {
            decoder.clear();
            continue;
        }
        if ((decoder == QStringLiteral("D3D11")
             || decoder == QStringLiteral("DXVA"))
                && (!hasNvidiaCard() && !hasAmdCard() && !hasIntelCard()))
        {
            decoder.clear();
            continue;
        }
    }
    decoders.removeAll(QString());
    if (!decoders.contains(QStringLiteral("FFmpeg")))
        decoders << QStringLiteral("FFmpeg");
    return decoders;
}

bool SettingsManager::getLocalize() const
{
    return settings->value(QStringLiteral("localize"), true).toBool();
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
    unsigned int vol = volume;
    if (vol > 99)
        vol = 99;
    settings->setValue(QStringLiteral("volume"), vol);
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
    QStringList newDecoders;
    if (hasNvidiaCard() && decoders.contains(QStringLiteral("CUDA")))
        newDecoders << QStringLiteral("CUDA");
    if ((hasNvidiaCard() || hasAmdCard() || hasIntelCard())
            && decoders.contains(QStringLiteral("D3D11")))
        newDecoders << QStringLiteral("D3D11");
    if ((hasNvidiaCard() || hasAmdCard() || hasIntelCard())
            && decoders.contains(QStringLiteral("DXVA")))
        newDecoders << QStringLiteral("DXVA");
    newDecoders << QStringLiteral("FFmpeg");
    settings->setValue(QStringLiteral("decoders"), newDecoders);
}

void SettingsManager::setLocalize(bool enable)
{
    settings->setValue(QStringLiteral("localize"), enable);
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
