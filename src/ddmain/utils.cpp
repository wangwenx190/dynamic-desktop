#include "utils.h"

#include <QMutex>
#include <QApplication>
#include <QTextStream>
#include <QDir>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QProcess>
#include <QWidget>
#include <QTranslator>
#include <QLocale>
#ifndef BUILD_DD_STATIC
#include <QLibraryInfo>
#endif

#include <wtsapi32.h>
#include <userenv.h>
#include <tlhelp32.h>
#include <tchar.h>

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
    QFileInfoList fileList = subDir.entryInfoList(QDir::Files | QDir::NoSymLinks, QDir::Name);
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
                    newFileList.append(QDir::toNativeSeparators(fi.absoluteFilePath()));
            }
            else if (fileType.toLower() == QLatin1String("audio"))
                if (fi.suffix().toLower() == QLatin1String("mka"))
                    newFileList.append(QDir::toNativeSeparators(fi.absoluteFilePath()));
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

bool adminRun(const QString &path, const QString &params)
{
    if (path.isEmpty())
        return false;
    if (!QFileInfo::exists(path))
        return false;
    SHELLEXECUTEINFO execInfo{ sizeof(SHELLEXECUTEINFO) };
    execInfo.lpVerb = TEXT("runas");
    execInfo.lpFile = reinterpret_cast<const wchar_t *>(QDir::toNativeSeparators(path).utf16());
    execInfo.nShow = SW_HIDE;
    execInfo.lpParameters = params.isEmpty() ? nullptr : reinterpret_cast<const wchar_t *>(params.utf16());
    return ShellExecuteEx(&execInfo);
}

bool launchSession1Process(const QString &path, const QString &params)
{
    if (path.isEmpty())
        return false;
    if (!QFileInfo::exists(path))
        return false;
    const QString dir = QDir::toNativeSeparators(QFileInfo(path).canonicalPath());
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { nullptr };
    si.cb = sizeof(si);
    DWORD dwSessionID = WTSGetActiveConsoleSessionId();
    HANDLE hToken = nullptr;
    if (WTSQueryUserToken(dwSessionID, &hToken) == FALSE)
        return false;
    HANDLE hDuplicatedToken = nullptr;
    if (DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, nullptr, SecurityIdentification, TokenPrimary, &hDuplicatedToken) == FALSE)
        return false;
    LPVOID lpEnvironment = nullptr;
    if (CreateEnvironmentBlock(&lpEnvironment, hDuplicatedToken, FALSE) == FALSE)
        return false;
    wchar_t *wcstring = nullptr;
    if (!params.isEmpty())
    {
        QByteArray byteArray = params.toLocal8Bit();
        char *param = byteArray.data();
        size_t newsize = strlen(param) + 1;
        wcstring = new wchar_t[newsize];
        size_t convertedChars = 0;
        mbstowcs_s(&convertedChars, wcstring, newsize, param, _TRUNCATE);
    }
    if (CreateProcessAsUser(hDuplicatedToken,
                            reinterpret_cast<const wchar_t *>(QDir::toNativeSeparators(path).utf16()),
                            params.isEmpty() ? nullptr : wcstring, nullptr, nullptr, FALSE,
                            NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
                            lpEnvironment,
                            reinterpret_cast<const wchar_t *>(dir.utf16()),
                            &si, &pi) == FALSE)
        return false;
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

int Exit(int resultCode, bool trulyExit, HANDLE mutex, HWND wallpaper)
{
    if (translator != nullptr)
    {
        delete translator;
        translator = nullptr;
    }
    if (mutex != nullptr)
    {
        ReleaseMutex(mutex);
        CloseHandle(mutex);
    }
    if (wallpaper != nullptr)
        ShowWindow(wallpaper, SW_HIDE);
    if (trulyExit)
        QCoreApplication::exit(resultCode);
    return resultCode;
}

bool run(const QString &path, const QStringList &params, bool needAdmin)
{
    if (path.isEmpty())
        return false;
    if (!QFileInfo::exists(path))
        return false;
    QString paramsInAll;
    if (!params.isEmpty())
        paramsInAll = params.join(QLatin1Char(' '));
    if (needAdmin)
        return adminRun(path, paramsInAll);
    DWORD processId = GetCurrentProcessId();
    DWORD sessionId;
    if (!ProcessIdToSessionId(processId, &sessionId))
        return false;
    if ((sessionId == static_cast<DWORD>(0)) || (sessionId != static_cast<DWORD>(1)))
        return launchSession1Process(path, paramsInAll);
    return QProcess::startDetached(QDir::toNativeSeparators(path), params, QDir::toNativeSeparators(QFileInfo(path).canonicalPath()));
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
#ifdef BUILD_DD_STATIC
    QString qmDir = QStringLiteral(":/i18n");
#else
    QString qmDir = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
    translator = new QTranslator();
    if (language == QStringLiteral("auto"))
    {
        if (translator->load(QLocale(), prefix, QStringLiteral("_"), qmDir))
            return QCoreApplication::installTranslator(translator);
    }
    else
    {
        const QString fileName = QStringLiteral("%0_%1").arg(prefix).arg(language);
        if (translator->load(fileName, qmDir))
            return QCoreApplication::installTranslator(translator);
    }
    delete translator;
    translator = nullptr;
    return false;
}

/*bool killProcess(const QString &name)
{
    if (name.isEmpty())
        return false;
    PROCESSENTRY32 pe;
    DWORD id = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe))
        return false;
    while (true)
    {
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32Next(hSnapshot, &pe) == FALSE)
            break;
        if (_tcscmp(pe.szExeFile, reinterpret_cast<const wchar_t *>(name.utf16())) == 0)
        {
            id = pe.th32ProcessID;
            break;
        }
    }
    CloseHandle(hSnapshot);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
    if (hProcess != nullptr)
    {
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
    }
    return true;
}*/

}
