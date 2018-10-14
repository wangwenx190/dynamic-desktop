#include <qtservice.h>
#include <utils.h>

#include <Windows.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>

class DDSvc : public QtService<QCoreApplication>
{
public:
    DDSvc(int argc, char **argv);

protected:
    void start() override;
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
    if ((mutex == nullptr) || (GetLastError() != ERROR_ALREADY_EXISTS))
    {
        ReleaseMutex(mutex);
        CloseHandle(mutex);
        if (QFileInfo::exists(path))
            Utils::launchSession1Process(QDir::toNativeSeparators(path), QStringLiteral("--launch"));
    }
    else
        ReleaseMutex(mutex);
    qApp->quit();
}

int main(int argc, char **argv)
{
    DDSvc svc(argc, argv);
    return svc.exec();
}
