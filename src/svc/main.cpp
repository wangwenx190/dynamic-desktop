#include <qtservice.h>

#include <Windows.h>
#include <wtsapi32.h>
#include <userenv.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>

class DDSvc : public QtService<QCoreApplication>
{
public:
    DDSvc(int argc, char **argv);

protected:
    void start() override;

private:
    bool launchSession1Process(const QString &path);
};

DDSvc::DDSvc(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, QStringLiteral("Dynamic Desktop Service"))
{
    setServiceDescription(QStringLiteral("A service to help Dynamic Desktop startup faster."));
    setStartupType(QtServiceController::AutoStartup);
}

void DDSvc::start()
{
    QString path = QCoreApplication::applicationDirPath() + QStringLiteral("/dd");
#ifdef WIN64
    path += QStringLiteral("64");
#endif
#ifdef _DEBUG
    path += QStringLiteral("d");
#endif
    path += QStringLiteral(".exe");
    HANDLE mutex = CreateMutex(nullptr, FALSE, TEXT(DD_MUTEX));
    if ((mutex != nullptr) && (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        ReleaseMutex(mutex);
        qApp->quit();
    }
    ReleaseMutex(mutex);
    CloseHandle(mutex);
    if (QFileInfo::exists(path))
        launchSession1Process(QDir::toNativeSeparators(path));
    qApp->quit();
}

bool DDSvc::launchSession1Process(const QString &path)
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
    if (CreateProcessAsUser(hDuplicatedToken,
                            reinterpret_cast<const wchar_t *>(QDir::toNativeSeparators(path).utf16()),
                            nullptr, nullptr, nullptr, FALSE,
                            NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
                            lpEnvironment,
                            reinterpret_cast<const wchar_t *>(dir.utf16()),
                            &si, &pi) == FALSE)
        return false;
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

int main(int argc, char **argv)
{
    DDSvc svc(argc, argv);
    return svc.exec();
}
