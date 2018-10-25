#include "settingsmanager.h"

#include <QDir>
#include <QUrl>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QCoreApplication>
#include <QStandardPaths>

SettingsManager *SettingsManager::getInstance()
{
    static SettingsManager settingsManager;
    return &settingsManager;
}

QStringList SettingsManager::defaultDecoders() const
{
    return QStringList()
            << QStringLiteral("CUDA")
            << QStringLiteral("D3D11")
            << QStringLiteral("DXVA")
            << QStringLiteral("FFmpeg");
}

QStringList SettingsManager::supportedMimeTypes() const
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

QString SettingsManager::getUrl() const
{
    const QStringList history = getHistory();
    if (history.isEmpty())
        return QString();
    const QString& path = history.constFirst();
    if (path.isEmpty())
        return QString();
    if (QFileInfo::exists(path))
        if (QFileInfo(path).isDir())
            return QString();
        else if (QFileInfo(path).isFile())
            return QDir::toNativeSeparators(path);
    QUrl url(path);
    if (!url.isValid())
        return QString();
    if (url.isLocalFile())
        return url.toLocalFile();
    return QUrl::fromPercentEncoding(url.toEncoded());
}

QString SettingsManager::lastDir() const
{
    const QString url = getUrl();
    if (!url.isEmpty())
    {
        const QString dir = QFileInfo(url).canonicalPath();
        if (QFileInfo::exists(dir) && QFileInfo(dir).isDir())
            return QDir::toNativeSeparators(dir);
    }
    for (auto& dirPath : QStandardPaths::standardLocations(QStandardPaths::MoviesLocation))
        if (QFileInfo::exists(dirPath) && QFileInfo(dirPath).isDir())
            return QDir::toNativeSeparators(dirPath);
    return QStringLiteral(".");
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
    return settings->value(QStringLiteral("decoders"), defaultDecoders()).toStringList();
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

QString SettingsManager::getSkin() const
{
    return settings->value(QStringLiteral("skin"), QStringLiteral("Default")).toString();
}

QString SettingsManager::getLanguage() const
{
    return settings->value(QStringLiteral("language"), QStringLiteral("auto")).toString();
}

int SettingsManager::getRenderer() const
{
    return settings->value(QStringLiteral("renderer"), QtAV_VId_GLWidget2).toInt();
}

QString SettingsManager::getImageQuality() const
{
    return settings->value(QStringLiteral("quality"), QStringLiteral("best")).toString().toLower();
}

QStringList SettingsManager::getHistory() const
{
    QStringList history;
    const int size = settings->beginReadArray(QStringLiteral("history"));
    if (size < 1)
    {
        settings->endArray();
        return history;
    }
    int first = size > static_cast<int>(getHistoryMax()) ? size - static_cast<int>(getHistoryMax()) : 0;
    for (int i = (size - 1); i != (first - 1); --i)
    {
        settings->setArrayIndex(i);
        const QString path = settings->value(QStringLiteral("path")).toString();
        if (QFileInfo::exists(path))
        {
            history.append(QDir::toNativeSeparators(path));
            if (!isHistoryEnabled())
                break;
        }
    }
    settings->endArray();
    return history;
}

bool SettingsManager::isHistoryEnabled() const
{
    return settings->value(QStringLiteral("savehistory"), true).toBool();
}

quint32 SettingsManager::getHistoryMax() const
{
    int max = settings->value(QStringLiteral("historymax"), 20).toInt();
    max = max < 1 ? 20 : max;
    return max;
}

bool SettingsManager::getAutoCheckUpdate() const
{
    return settings->value(QStringLiteral("autoupdate"), false).toBool();
}

void SettingsManager::setUrl(const QString &url)
{
    QStringList history = getHistory();
    if (!history.isEmpty())
        history.removeAll(QDir::toNativeSeparators(url));
    history.append(QDir::toNativeSeparators(url));
    setHistory(history);
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

void SettingsManager::setSkin(const QString &skin)
{
    settings->setValue(QStringLiteral("skin"), skin);
}

void SettingsManager::setLanguage(const QString &lang)
{
    settings->setValue(QStringLiteral("language"), lang);
}

void SettingsManager::setRenderer(int vid)
{
    settings->setValue(QStringLiteral("renderer"), vid);
}

void SettingsManager::setImageQuality(const QString &quality)
{
    settings->setValue(QStringLiteral("quality"), quality);
}

void SettingsManager::setHistory(const QStringList &history)
{
    if (history.isEmpty())
        return;
    quint32 first = history.count() > static_cast<int>(getHistoryMax()) ? history.count() - static_cast<int>(getHistoryMax()) : 0;
    settings->beginWriteArray(QStringLiteral("history"));
    for (quint32 i = first, j = 0; i != history.count(); ++i, ++j)
    {
        settings->setArrayIndex(j);
        if (isHistoryEnabled())
            settings->setValue(QStringLiteral("path"), QDir::toNativeSeparators(history.at(i)));
        else
        {
            settings->setValue(QStringLiteral("path"), QDir::toNativeSeparators(history.constLast()));
            break;
        }
    }
    settings->endArray();
}

void SettingsManager::setHistoryEnabled(bool enabled)
{
    settings->setValue(QStringLiteral("savehistory"), enabled);
}

void SettingsManager::setHistoryMax(int max)
{
    quint32 newMax = max < 1 ? 20 : max;
    settings->setValue(QStringLiteral("historymax"), newMax);
}

void SettingsManager::setAutoCheckUpdate(bool enabled)
{
    settings->setValue(QStringLiteral("autoupdate"), enabled);
}

SettingsManager::SettingsManager()
{
    QString iniPath = QCoreApplication::applicationDirPath();
    iniPath += QStringLiteral("/config.ini");
    settings = new QSettings(iniPath, QSettings::IniFormat);
    settings->beginGroup(QStringLiteral("dd"));
}

SettingsManager::~SettingsManager()
{
    settings->endGroup();
    delete settings;
}
