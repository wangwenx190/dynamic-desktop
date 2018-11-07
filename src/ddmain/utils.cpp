#include "utils.h"
#include <Win32Utils>

#include <QMutex>
#include <QApplication>
#include <QTextStream>
#include <QDir>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QWidget>
#include <QTranslator>
#include <QLocale>
#include <QtAVWidgets>

namespace Utils
{

QTranslator *translator = nullptr;

void fileLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString msgType;
    switch (type)
    {
    case QtDebugMsg:
        msgType = QStringLiteral("DEBUG");
        break;
    case QtInfoMsg:
        msgType = QStringLiteral("INFORMATION");
        break;
    case QtWarningMsg:
        msgType = QStringLiteral("WARNING");
        break;
    case QtCriticalMsg:
        msgType = QStringLiteral("CRITICAL");
        break;
    case QtFatalMsg:
        msgType = QStringLiteral("FATAL");
        break;
        /*case QtSystemMsg:
        msgType = QStringLiteral("SYSTEM");
        break;*/
    default:
        msgType = QStringLiteral("DEBUG");
        break;
    }
    QString messageStr = QStringLiteral("%0\t%1\t%2\t%3\t%4")
            .arg(msgType).arg(msg).arg(context.file).arg(context.line).arg(context.function);
    QString logPath = QCoreApplication::applicationDirPath();
    logPath += QStringLiteral("/debug.log");
    QFile file(logPath);
    if (file.open(QFile::WriteOnly | QFile::Append | QFile::Text))
    {
        QTextStream ts(&file);
        ts << messageStr << QLatin1Char('\n');
        file.close();
    }
    mutex.unlock();
}

QStringList externalFilesToLoad(const QFileInfo &originalMediaFile, const QString &fileType)
{
    if (!originalMediaFile.exists() || originalMediaFile.isDir() || fileType.isEmpty())
        return QStringList();
    QDir subDir(originalMediaFile.absoluteDir());
    QFileInfoList fileList = subDir.entryInfoList(QDir::Files, QDir::Name);
    if (fileList.count() < 2)
        return QStringList();
    const QString fileBaseName = originalMediaFile.baseName().toLower();
    QStringList newFileList;
    for (auto& fi : fileList)
    {
        if (fi.absoluteFilePath() == originalMediaFile.absoluteFilePath())
            continue;
        const QString newBaseName = fi.baseName().toLower();
        if (newBaseName == fileBaseName)
            if (fileType.toLower() == QLatin1String("sub"))
            {
                if (fi.suffix().toLower() == QLatin1String("ass")
                        || fi.suffix().toLower() == QLatin1String("ssa")
                        || fi.suffix().toLower() == QLatin1String("srt")
                        || fi.suffix().toLower() == QLatin1String("sub"))
                    newFileList.append(QDir::toNativeSeparators(QDir::cleanPath(fi.absoluteFilePath())));
            }
            else if (fileType.toLower() == QLatin1String("audio"))
                if (fi.suffix().toLower() == QLatin1String("mka"))
                    newFileList.append(QDir::toNativeSeparators(QDir::cleanPath(fi.absoluteFilePath())));
    }
    return newFileList;
}

void moveToCenter(QObject *window)
{
    if (!window)
        return;
    auto win = qobject_cast<QWidget *>(window);
    quint32 screenWidth = QApplication::desktop()->screenGeometry(win).width();
    quint32 screenHeight = QApplication::desktop()->screenGeometry(win).height();
    quint32 windowWidth = win->width();
    quint32 windowHeight = win->height();
    quint32 newX = (screenWidth - windowWidth) / 2;
    quint32 newY = (screenHeight - windowHeight) / 2;
    win->move(newX, newY);
}

bool win32Run(const QString &path, const QString &params = QString(), bool needAdmin = false, bool needHide = false)
{
    if (path.isEmpty())
        return false;
    if (!QFileInfo::exists(path))
        return false;
    SHELLEXECUTEINFO execInfo{ sizeof(SHELLEXECUTEINFO) };
    execInfo.lpVerb = needAdmin ? TEXT("runas") : nullptr;
    execInfo.lpFile = reinterpret_cast<LPCTSTR>(QDir::toNativeSeparators(QDir::cleanPath(path)).utf16());
    execInfo.nShow = needHide ? SW_HIDE : SW_SHOW;
    execInfo.lpParameters = params.isEmpty() ? nullptr : reinterpret_cast<LPCTSTR>(params.utf16());
    return ShellExecuteEx(&execInfo);
}

bool run(const QString &path, const QStringList &params, bool needAdmin, bool needHide)
{
    if (path.isEmpty())
        return false;
    if (!QFileInfo::exists(path))
        return false;
    QString paramsInAll;
    if (!params.isEmpty())
        paramsInAll = params.join(QLatin1Char(' '));
    if (needAdmin)
        return win32Run(path, paramsInAll, true, needHide);
    if (!Win32Utils::isSession1Process())
#ifdef UNICODE
        return Win32Utils::launchSession1Process(reinterpret_cast<LPCWSTR>(QDir::toNativeSeparators(QDir::cleanPath(path)).utf16()), paramsInAll.isEmpty() ? nullptr : reinterpret_cast<LPCWSTR>(paramsInAll.utf16()));
#else
        return Win32Utils::launchSession1Process(QDir::toNativeSeparators(QDir::cleanPath(path)).toLocal8Bit().constData(), paramsInAll.isEmpty() ? nullptr : paramsInAll.toLocal8Bit().constData());
#endif
    return win32Run(path, paramsInAll, false, needHide);
}

bool isVideo(const QString &fileName)
{
    if (fileName.isEmpty())
        return false;
    // This method is stupid.
    // How to judge whether it's a video file or not?
    // FIXME
    return fileName.endsWith(QStringLiteral(".mp4"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".avi"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".mov"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".wmv"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".rm"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".rmvb"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".mkv"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".flv"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".asf"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".3gp"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".ts"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".swf"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".vob"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".dat"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".mpeg"), Qt::CaseInsensitive);
}

bool isAudio(const QString &fileName)
{
    if (fileName.isEmpty())
        return false;
    // Stupid method
    // TODO: FIXME
    return fileName.endsWith(QStringLiteral(".mp3"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".flac"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".ape"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".wav"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".ogg"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".midi"), Qt::CaseInsensitive);
}

bool isPicture(const QString &fileName)
{
    if (fileName.isEmpty())
        return false;
    // Stupid method
    // FIXME
    return fileName.endsWith(QStringLiteral(".bmp"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".png"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".jpg"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".jpeg"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".webp"), Qt::CaseInsensitive)
            || fileName.endsWith(QStringLiteral(".gif"), Qt::CaseInsensitive);
}

bool installTranslation(const QString &language, const QString &prefix)
{
    if (language.isEmpty() || prefix.isEmpty())
        return false;
    if (translator != nullptr)
    {
        QCoreApplication::removeTranslator(translator);
        delete translator;
        translator = nullptr;
    }
    const QString qmDir = QStringLiteral(":/i18n");
    translator = new QTranslator();
    if (language == QStringLiteral("auto"))
    {
        if (translator->load(QLocale(), prefix, QStringLiteral("_"), qmDir))
            return QCoreApplication::installTranslator(translator);
    }
    else
    {
        if (language.contains(QLatin1Char('/')) || language.contains(QLatin1Char('\\')))
        {
            if (translator->load(language))
                return QCoreApplication::installTranslator(translator);
        }
        else
        {
            const QString fileName = QStringLiteral("%0_%1").arg(prefix).arg(language);
            if (translator->load(fileName, qmDir))
                return QCoreApplication::installTranslator(translator);
        }
    }
    delete translator;
    translator = nullptr;
    return false;
}

int getVideoRendererId(const VideoRendererId vid)
{
    QtAV::VideoRendererId id;
    switch (vid)
    {
    case VideoRendererId::OpenGLWidget:
        id = QtAV::VideoRendererId_OpenGLWidget;
        break;
    case VideoRendererId::Widget:
        id = QtAV::VideoRendererId_Widget;
        break;
    case VideoRendererId::GDI:
        id = QtAV::VideoRendererId_GDI;
        break;
    case VideoRendererId::Direct2D:
        id = QtAV::VideoRendererId_Direct2D;
        break;
    default:
        id = QtAV::VideoRendererId_GLWidget2;
        break;
    }
    return static_cast<int>(id);
}

}
