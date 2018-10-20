#include "core.h"

#include <QtService>
#include <Utils>

#include <Windows.h>

#include <QApplication>
#include <QFileInfo>

class DDSvc : public QtService<QApplication>
{
public:
    DDSvc(int argc, char **argv);

protected:
    void start() override;
};

DDSvc::DDSvc(int argc, char **argv)
    : QtService<QApplication>(argc, argv, QStringLiteral("Dynamic Desktop Service"))
{
    setServiceDescription(QStringLiteral("A service to help Dynamic Desktop startup faster."));
    setStartupType(QtServiceController::AutoStartup);
    setStartupArguments(QStringList() << QStringLiteral("--service"));
}

void DDSvc::start()
{
    HANDLE serviceMutex = CreateMutex(nullptr, FALSE, TEXT("wangwenx190.DynamicDesktop.Service.1000.AppMutex"));
    if ((serviceMutex != nullptr) && (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        ReleaseMutex(serviceMutex);
        qApp->quit();
    }
    HANDLE playerMutex = CreateMutex(nullptr, FALSE, TEXT("wangwenx190.DynamicDesktop.Player.1000.AppMutex"));
    if ((playerMutex == nullptr) || (GetLastError() != ERROR_ALREADY_EXISTS))
    {
        ReleaseMutex(playerMutex);
        CloseHandle(playerMutex);
        Utils::run(QCoreApplication::applicationFilePath(), QStringList() << QStringLiteral("--controller") << QStringLiteral("--launch"));
    }
    else
        ReleaseMutex(playerMutex);
    ReleaseMutex(serviceMutex);
    CloseHandle(serviceMutex);
    qApp->quit();
}

int serviceMain(int argc, char **argv)
{
    DDSvc svc(argc, argv);
    return svc.exec();
}
