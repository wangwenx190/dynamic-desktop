#include "utils.h"

#include <qtservice.h>
#include <wallpaper.h>

#include <QMutex>
#include <QApplication>
#include <QTextStream>
#include <QDir>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QProcess>
#include <QWidget>

#include <wtsapi32.h>
#include <userenv.h>

namespace Utils
{

static HANDLE mutex = nullptr;

void Exit(int resultCode)
{
    preExit();
    exit(resultCode);
}

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
    unsigned int screenWidth = QApplication::desktop()->screenGeometry(win).width();
    unsigned int screenHeight = QApplication::desktop()->screenGeometry(win).height();
    unsigned int windowWidth = win->width();
    unsigned int windowHeight = win->height();
    unsigned int newX = (screenWidth - windowWidth) / 2;
    unsigned int newY = (screenHeight - windowHeight) / 2;
    win->move(newX, newY);
}

void preExit()
{
    if (mutex != nullptr)
    {
        ReleaseMutex(mutex);
        CloseHandle(mutex);
    }
    if (Wallpaper::getWorkerW() != nullptr)
        ShowWindow(Wallpaper::getWorkerW(), SW_HIDE);
}

int ExitProgram(int resultCode)
{
    preExit();
    return resultCode;
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

bool checkUpdate(bool hide)
{
    const QString updaterDir = QCoreApplication::applicationDirPath();
    QString updaterPath = updaterDir + QStringLiteral("/ddudt");
#ifdef WIN64
    updaterPath += QStringLiteral("64");
#endif
#ifdef _DEBUG
    updaterPath += QStringLiteral("d");
#endif
    updaterPath += QStringLiteral(".exe");
    if (!QFileInfo::exists(updaterPath))
        return false;
    QStringList arguments = QCoreApplication::arguments();
    arguments.takeFirst();
    if (hide)
        arguments << QStringLiteral("--no-gui");
    return QProcess::startDetached(QDir::toNativeSeparators(updaterPath), arguments, QDir::toNativeSeparators(updaterDir));
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
    wchar_t *wcstring;
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

bool isAutoStart(const QString &name)
{
    QString serviceName = name.isEmpty() ? QCoreApplication::applicationName() : name;
    QtServiceController controller(serviceName);
    return controller.isInstalled();
}

bool setAutoStart(bool enable)
{
    QString servicePath = QCoreApplication::applicationDirPath() + QStringLiteral("/ddsvc");
#ifdef WIN64
    servicePath += QStringLiteral("64");
#endif
#ifdef _DEBUG
    servicePath += QStringLiteral("d");
#endif
    servicePath += QStringLiteral(".exe");
    if (!QFileInfo::exists(servicePath))
        return false;
    if (enable && !isAutoStart())
        return adminRun(QDir::toNativeSeparators(servicePath), QStringLiteral("-i"));
    else if (!enable && isAutoStart())
        return adminRun(QDir::toNativeSeparators(servicePath), QStringLiteral("-u"));
    return false;
}

HANDLE getAppMutex()
{
    return mutex;
}

void setAppMutex(HANDLE appMutex)
{
    mutex = appMutex;
}

}
