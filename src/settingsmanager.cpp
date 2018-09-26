#include "settingsmanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QUrl>
#include <QFileInfo>
#include <QSettings>
#include <QMimeDatabase>

SettingsManager *SettingsManager::getInstance()
{
    static SettingsManager settingsManager;
    return &settingsManager;
}

void SettingsManager::unregAutostart()
{
    const QString key = QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    QSettings set(key, QSettings::NativeFormat);
    if (set.contains(QStringLiteral("Dynamic Desktop")))
        set.remove(QStringLiteral("Dynamic Desktop"));
}

bool SettingsManager::regAutostart()
{
    unregAutostart();
    const QString key = QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    QSettings set(key, QSettings::NativeFormat);
    if (set.status() != QSettings::NoError)
        return false;
    QString value = QLatin1Char('"') + QCoreApplication::applicationFilePath() + QLatin1Char('"');
    set.setValue(QStringLiteral("Dynamic Desktop"), QDir::toNativeSeparators(value));
    return true;
}

/*bool SettingsManager::hasNvidiaCard() const
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
}*/

QStringList SettingsManager::defaultDecoders() const
{
    QStringList decoders;
    //if (hasNvidiaCard())
        decoders << QStringLiteral("CUDA");
    //if (hasNvidiaCard() || hasAmdCard() || hasIntelCard())
        decoders << QStringLiteral("D3D11") << QStringLiteral("DXVA");
    decoders << QStringLiteral("FFmpeg");
    return decoders;
}

QStringList SettingsManager::supportedMimeTypes()
{
    return (QStringList()
            << QStringLiteral("audio/ac3")
            << QStringLiteral("audio/eac3")
            << QStringLiteral("audio/vnd.dolby.mlp")
            << QStringLiteral("audio/vnd.dts")
            << QStringLiteral("audio/vnd.dts.hd")
            << QStringLiteral("audio/wav")
            << QStringLiteral("audio/aiff")
            << QStringLiteral("audio/amr")
            << QStringLiteral("audio/amr-wb")
            << QStringLiteral("audio/basic")
            << QStringLiteral("audio/x-ape")
            << QStringLiteral("audio/x-wavpack")
            << QStringLiteral("audio/x-shorten")
            << QStringLiteral("video/vnd.dlna.mpeg-tts")
            << QStringLiteral("audio/vnd.dlna.adts")
            << QStringLiteral("audio/mpeg")
            << QStringLiteral("video/mpeg")
            << QStringLiteral("video/dvd")
            << QStringLiteral("video/mp4")
            << QStringLiteral("audio/mp4")
            << QStringLiteral("audio/aac")
            << QStringLiteral("audio/flac")
            << QStringLiteral("audio/ogg")
            << QStringLiteral("video/ogg")
            << QStringLiteral("application/ogg")
            << QStringLiteral("video/x-matroska")
            << QStringLiteral("audio/x-matroska")
            << QStringLiteral("video/webm")
            << QStringLiteral("audio/webm")
            << QStringLiteral("video/avi")
            << QStringLiteral("video/x-msvideo")
            << QStringLiteral("video/flc")
            << QStringLiteral("application/gxf")
            << QStringLiteral("application/mxf")
            << QStringLiteral("audio/x-ms-wma")
            << QStringLiteral("video/x-ms-wm")
            << QStringLiteral("video/x-ms-wmv")
            << QStringLiteral("video/x-ms-asf")
            << QStringLiteral("video/x-flv")
            << QStringLiteral("video/mp4")
            << QStringLiteral("audio/mp4")
            << QStringLiteral("video/quicktime")
            << QStringLiteral("application/vnd.rn-realmedia")
            << QStringLiteral("application/vnd.rn-realmedia-vbr")
            << QStringLiteral("audio/vnd.rn-realaudio")
            << QStringLiteral("audio/3gpp")
            << QStringLiteral("audio/3gpp2")
            << QStringLiteral("video/3gpp")
            << QStringLiteral("video/3gpp2")
            << QStringLiteral("audio/x-mpegurl")
            << QStringLiteral("audio/x-scpls"));
}

QStringList SettingsManager::supportedSuffixes()
{
    QStringList mSupportedSuffixes;
    QMimeDatabase mMimeDatabase;
    const QStringList mSupportedMimeTypes = supportedMimeTypes();
    for (auto& mFileType : mSupportedMimeTypes)
    {
        const QStringList mSuffixes = mMimeDatabase.mimeTypeForName(mFileType).suffixes();
        for (auto mSuffix : mSuffixes)
        {
            mSuffix.prepend(QStringLiteral("*."));
            mSupportedSuffixes << mSuffix;
        }
    }
    return mSupportedSuffixes;
}

QString SettingsManager::getUrl() const
{
    QSettings settings(iniPath, QSettings::IniFormat);
    QString path = settings.value(QStringLiteral("dd/url"), QString()).toString();
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
    QSettings settings(iniPath, QSettings::IniFormat);
    return settings.value(QStringLiteral("dd/mute"), false).toBool();
}

unsigned int SettingsManager::getVolume() const
{
    QSettings settings(iniPath, QSettings::IniFormat);
    int vol = settings.value(QStringLiteral("dd/volume"), 9).toInt();
    if (vol < 0)
        vol = 0;
    if (vol > 99)
        vol = 99;
    return static_cast<unsigned int>(vol);
}

bool SettingsManager::getAutostart() const
{
    QSettings settings(iniPath, QSettings::IniFormat);
    return settings.value(QStringLiteral("dd/autostart"), false).toBool();
}

bool SettingsManager::getHwdec() const
{
    QSettings settings(iniPath, QSettings::IniFormat);
    return settings.value(QStringLiteral("dd/hwdec"), true).toBool();
}

QStringList SettingsManager::getDecoders() const
{
    QSettings settings(iniPath, QSettings::IniFormat);
    QStringList decoders = settings.value(QStringLiteral("dd/decoders"), defaultDecoders()).toStringList();
    /*for (auto& decoder : decoders)
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
        decoders << QStringLiteral("FFmpeg");*/
    return decoders;
}

bool SettingsManager::getLocalize() const
{
    QSettings settings(iniPath, QSettings::IniFormat);
    return settings.value(QStringLiteral("dd/localize"), true).toBool();
}

void SettingsManager::setUrl(const QString &url)
{
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue(QStringLiteral("dd/url"), url);
}

void SettingsManager::setMute(bool mute)
{
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue(QStringLiteral("dd/mute"), mute);
}

void SettingsManager::setVolume(unsigned int volume)
{
    unsigned int vol = volume;
    if (vol > 99)
        vol = 99;
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue(QStringLiteral("dd/volume"), vol);
}

void SettingsManager::setAutostart(bool enable)
{
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue(QStringLiteral("dd/autostart"), enable);
}

void SettingsManager::setHwdec(bool enable)
{
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue(QStringLiteral("dd/hwdec"), enable);
}

void SettingsManager::setDecoders(const QStringList &decoders)
{
    /*QStringList newDecoders;
    if (hasNvidiaCard() && decoders.contains(QStringLiteral("CUDA")))
        newDecoders << QStringLiteral("CUDA");
    if ((hasNvidiaCard() || hasAmdCard() || hasIntelCard())
            && decoders.contains(QStringLiteral("D3D11")))
        newDecoders << QStringLiteral("D3D11");
    if ((hasNvidiaCard() || hasAmdCard() || hasIntelCard())
            && decoders.contains(QStringLiteral("DXVA")))
        newDecoders << QStringLiteral("DXVA");
    newDecoders << QStringLiteral("FFmpeg");*/
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue(QStringLiteral("dd/decoders"), /*newDecoders*/decoders);
}

void SettingsManager::setLocalize(bool enable)
{
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue(QStringLiteral("dd/localize"), enable);
}

SettingsManager::SettingsManager()
{
    iniPath = QCoreApplication::applicationFilePath();
    if (iniPath.endsWith(QStringLiteral(".exe"), Qt::CaseInsensitive))
        iniPath = iniPath.remove(iniPath.lastIndexOf(QLatin1Char('.')), 4);
    iniPath += QStringLiteral(".ini");
}
