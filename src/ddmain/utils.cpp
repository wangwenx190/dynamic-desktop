#include "utils.h"
#include <Win32Utils>

#include <QApplication>
#include <QDir>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QWidget>
#include <QtAVWidgets>
#include <QOperatingSystemVersion>
#ifndef DD_NO_WIN_EXTRAS
#include <QtWin>
#endif

namespace Utils
{

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
#ifdef UNICODE
    execInfo.lpFile = reinterpret_cast<LPCWSTR>(QDir::toNativeSeparators(QDir::cleanPath(path)).utf16());
    execInfo.lpParameters = params.isEmpty() ? nullptr : reinterpret_cast<LPCWSTR>(params.utf16());
#else
    execInfo.lpFile = QDir::toNativeSeparators(QDir::cleanPath(path)).toLocal8Bit().constData();
    execInfo.lpParameters = params.isEmpty() ? nullptr : params.toLocal8Bit().constData();
#endif
    execInfo.nShow = needHide ? SW_HIDE : SW_SHOW;
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

void activateWindow(QObject *window, bool moveCenter)
{
    if (!window)
        return;
    if (moveCenter)
        moveToCenter(window);
    auto win = qobject_cast<QWidget *>(window);
    if (win->isHidden())
        win->show();
    if (!win->isActiveWindow())
        win->setWindowState(win->windowState() & ~Qt::WindowMinimized);
    if (!win->isActiveWindow())
    {
        win->raise();
        win->activateWindow();
    }
}

bool enableBlurBehindWindow(QObject *window)
{
    if (window == nullptr)
        return false;
    if (QOperatingSystemVersion::current() < QOperatingSystemVersion::Windows7)
        return false;
    auto win = qobject_cast<QWidget *>(window);
    if (QOperatingSystemVersion::current() < QOperatingSystemVersion::Windows8_1)
    {
#ifndef DD_NO_WIN_EXTRAS
        if (QtWin::isCompositionEnabled())
        {
            QtWin::extendFrameIntoClientArea(win, -1, -1, -1, -1);
            win->setAttribute(Qt::WA_TranslucentBackground, true);
            win->setAttribute(Qt::WA_NoSystemBackground, false);
            QtWin::enableBlurBehindWindow(win);
        }
        else
        {
            QtWin::resetExtendedFrame(win);
            win->setAttribute(Qt::WA_TranslucentBackground, false);
            QString css = qApp->styleSheet();
            if (!css.isEmpty())
            {
                css = css.replace(QStringLiteral("0.6"), QStringLiteral("1"));
                qApp->setStyleSheet(css);
            }
            QtWin::disableBlurBehindWindow(win);
        }
#endif
        return true;
    }
    Win32Utils::enableBlurBehindWindow(reinterpret_cast<HWND>(win->winId()));
    return true;
}

}
