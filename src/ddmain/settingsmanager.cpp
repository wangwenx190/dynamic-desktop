#include "settingsmanager.h"

#include <QDir>
#include <QUrl>
#include <QFileInfo>
#ifndef DD_NO_MIME_TYPE
#include <QMimeDatabase>
#endif
#include <QCoreApplication>
#include <QStandardPaths>

SettingsManager *SettingsManager::getInstance()
{
    static SettingsManager settingsManager;
    return &settingsManager;
}

QStringList SettingsManager::getDefaultDecoders() const
{
    return QStringList()
            << QStringLiteral("CUDA")
            << QStringLiteral("D3D11")
            << QStringLiteral("DXVA")
            << QStringLiteral("FFmpeg");
}

#ifndef DD_NO_MIME_TYPE
QStringList SettingsManager::getSupportedMimeTypes() const
{
    return QStringList()
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
            << QStringLiteral("audio/x-scpls");
}
#endif

QString SettingsManager::getLastFile() const
{
    const QString& path = settings->value(QStringLiteral("currentfile"), QString()).toString();
    if (path.isEmpty())
        return QString();
    if (QFileInfo::exists(path))
        if (QFileInfo(path).isDir())
            return QString();
        else if (QFileInfo(path).isFile())
            return QDir::toNativeSeparators(QDir::cleanPath(path));
    QUrl url(path);
    if (!url.isValid())
        return QString();
    if (url.isLocalFile())
        return url.toLocalFile();
    return QUrl::fromPercentEncoding(url.toEncoded());
}

QString SettingsManager::getLastDir() const
{
    const QString url = getLastFile();
    if (!url.isEmpty())
    {
        const QString dir = QDir::cleanPath(QFileInfo(url).absolutePath());
        if (QFileInfo::exists(dir) && QFileInfo(dir).isDir())
            return QDir::toNativeSeparators(dir);
    }
    for (auto& dirPath : QStandardPaths::standardLocations(QStandardPaths::MoviesLocation))
        if (QFileInfo::exists(dirPath) && QFileInfo(dirPath).isDir())
            return QDir::toNativeSeparators(QDir::cleanPath(dirPath));
    return QDir::toNativeSeparators(QDir::cleanPath(QCoreApplication::applicationDirPath()));
}

bool SettingsManager::getMute() const
{
    return settings->value(QStringLiteral("mute"), false).toBool();
}

quint32 SettingsManager::getVolume() const
{
    int vol = settings->value(QStringLiteral("volume"), 9).toInt();
    if (vol < 0)
        vol = 0;
    if (vol > 99)
        vol = 99;
    return static_cast<quint32>(vol);
}

bool SettingsManager::getHwdec() const
{
    return settings->value(QStringLiteral("hwdec"), false).toBool();
}

QStringList SettingsManager::getDecoders() const
{
    return settings->value(QStringLiteral("decoders"), getDefaultDecoders()).toStringList();
}

bool SettingsManager::getFitDesktop() const
{
    return settings->value(QStringLiteral("fit"), true).toBool();
}

bool SettingsManager::getSubtitle() const
{
    return settings->value(QStringLiteral("subtitle"), true).toBool();
}

QString SettingsManager::getCharset() const
{
    return settings->value(QStringLiteral("charset"), QStringLiteral("AutoDetect")).toString();
}

bool SettingsManager::getSubtitleAutoLoad() const
{
    return settings->value(QStringLiteral("subtitleautoload"), true).toBool();
}

bool SettingsManager::getAudioAutoLoad() const
{
    return settings->value(QStringLiteral("audioautoload"), true).toBool();
}

#ifndef DD_NO_CSS
QString SettingsManager::getSkin() const
{
    return settings->value(QStringLiteral("skin"), QStringLiteral("Default")).toString();
}
#endif

#ifndef DD_NO_TRANSLATIONS
QString SettingsManager::getLanguage() const
{
    return settings->value(QStringLiteral("language"), QStringLiteral("auto")).toString();
}
#endif

int SettingsManager::getRenderer() const
{
    return settings->value(QStringLiteral("renderer"), 0).toInt();
}

QString SettingsManager::getImageQuality() const
{
    return settings->value(QStringLiteral("quality"), QStringLiteral("best")).toString().toLower();
}

bool SettingsManager::getAutoCheckUpdate() const
{
    //return settings->value(QStringLiteral("autoupdate"), false).toBool();
    return false;
}

SettingsManager::PlaybackMode SettingsManager::getPlaybackMode() const
{
    int mode = settings->value(QStringLiteral("playbackmode"), PlaybackMode::RepeatCurrentFile).toInt();
    if (mode < 0)
        mode = 0;
    if (mode > 5)
        mode = 5;
    return static_cast<PlaybackMode>(mode);
}

QString SettingsManager::getCurrentPlaylistName() const
{
    return settings->value(QStringLiteral("currentplaylist"), QStringLiteral("Default")).toString();
}

QStringList SettingsManager::getAllFilesFromPlaylist(const QString &name) const
{
    settings->beginGroup(QStringLiteral("playlists"));
    QStringList playlist;
    int size = settings->beginReadArray(name);
    for (int i = 0; i != size; ++i)
    {
        settings->setArrayIndex(i);
        playlist.append(QDir::toNativeSeparators(QDir::cleanPath(settings->value(QStringLiteral("path"), QString()).toString())));
    }
    settings->endArray();
    settings->endGroup();
    return playlist;
}

QStringList SettingsManager::getAllPlaylistNames() const
{
    return settings->value(QStringLiteral("allplaylists"), QStringList() << QStringLiteral("Default")).toStringList();
}

void SettingsManager::setLastFile(const QString &url)
{
    settings->setValue(QStringLiteral("currentfile"), QDir::toNativeSeparators(QDir::cleanPath(url)));
}

void SettingsManager::setMute(bool mute)
{
    settings->setValue(QStringLiteral("mute"), mute);
}

void SettingsManager::setVolume(quint32 volume)
{
    quint32 vol = volume;
    if (vol > 99)
        vol = 99;
    settings->setValue(QStringLiteral("volume"), vol);
}

void SettingsManager::setHwdec(bool enable)
{
    settings->setValue(QStringLiteral("hwdec"), enable);
}

void SettingsManager::setDecoders(const QStringList &decoders)
{
    settings->setValue(QStringLiteral("decoders"), decoders);
}

void SettingsManager::setFitDesktop(bool fit)
{
    settings->setValue(QStringLiteral("fit"), fit);
}

void SettingsManager::setSubtitle(bool show)
{
    settings->setValue(QStringLiteral("subtitle"), show);
}

void SettingsManager::setCharset(const QString &charset)
{
    settings->setValue(QStringLiteral("charset"), charset);
}

void SettingsManager::setSubtitleAutoLoad(bool autoload)
{
    settings->setValue(QStringLiteral("subtitleautoload"), autoload);
}

void SettingsManager::setAudioAutoLoad(bool autoload)
{
    settings->setValue(QStringLiteral("audioautoload"), autoload);
}

#ifndef DD_NO_CSS
void SettingsManager::setSkin(const QString &skin)
{
    settings->setValue(QStringLiteral("skin"), skin);
}
#endif

#ifndef DD_NO_TRANSLATIONS
void SettingsManager::setLanguage(const QString &lang)
{
    settings->setValue(QStringLiteral("language"), lang);
}
#endif

void SettingsManager::setRenderer(int vid)
{
    settings->setValue(QStringLiteral("renderer"), vid);
}

void SettingsManager::setImageQuality(const QString &quality)
{
    settings->setValue(QStringLiteral("quality"), quality);
}

void SettingsManager::setAutoCheckUpdate(bool enabled)
{
    settings->setValue(QStringLiteral("autoupdate"), enabled);
}

void SettingsManager::setPlaybackMode(SettingsManager::PlaybackMode playbackMode)
{
    settings->setValue(QStringLiteral("playbackmode"), playbackMode);
}

void SettingsManager::setCurrentPlaylistName(const QString &name)
{
    settings->setValue(QStringLiteral("currentplaylist"), name);
}

void SettingsManager::setPlaylistFiles(const QString &name, const QStringList &files)
{
    settings->beginGroup(QStringLiteral("playlists"));
    settings->beginWriteArray(name);
    for (int i = 0; i != files.count(); ++i)
    {
        settings->setArrayIndex(i);
        settings->setValue(QStringLiteral("path"), QDir::toNativeSeparators(QDir::cleanPath(files.at(i))));
    }
    settings->endArray();
    settings->endGroup();
}

void SettingsManager::setAllPlaylistNames(const QStringList &names)
{
    settings->setValue(QStringLiteral("allplaylists"), names);
}

SettingsManager::SettingsManager()
{
    QString iniPath = QCoreApplication::applicationDirPath();
    iniPath += QStringLiteral("/config.ini");
    settings = new QSettings(QDir::toNativeSeparators(QDir::cleanPath(iniPath)), QSettings::IniFormat);
    settings->beginGroup(QStringLiteral("dd"));
}

SettingsManager::~SettingsManager()
{
    settings->endGroup();
    delete settings;
}
