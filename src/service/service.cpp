#include <QtService>
#include <Utils>

#include <Windows.h>

#include <QCoreApplication>

class DDSvc : public QtService<QCoreApplication>
{
public:
    DDSvc(int argc, char **argv);

protected:
    void start() override;
};

DDSvc::DDSvc(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, QStringLiteral("ddassvc"))
{
    setServiceDisplayName(QStringLiteral("Dynamic Desktop Auto Start Service"));
    setServiceDescription(QStringLiteral("Make Dynamic Desktop automatically run when the system starts. Dynamic Desktop will not auto start if you disabled this service."));
    setStartupType(QtServiceController::AutoStartup);
}

void DDSvc::start()
{
    HANDLE serviceMutex = CreateMutex(nullptr, FALSE, TEXT("wangwenx190.DynamicDesktop.Service.1000.AppMutex"));
    if ((serviceMutex != nullptr) && (GetLastError() == ERROR_ALREADY_EXISTS))
    {
        ReleaseMutex(serviceMutex);
        QCoreApplication::quit();
    }
    HANDLE playerMutex = CreateMutex(nullptr, FALSE, TEXT("wangwenx190.DynamicDesktop.Player.1000.AppMutex"));
    if ((playerMutex == nullptr) || (GetLastError() != ERROR_ALREADY_EXISTS))
    {
        ReleaseMutex(playerMutex);
        CloseHandle(playerMutex);
        QString controllerPath = QCoreApplication::applicationDirPath() + QStringLiteral("/ddmain");
#ifdef _DEBUG
        controllerPath += QStringLiteral("d");
#endif
        controllerPath += QStringLiteral(".exe");
        Utils::run(controllerPath);
    }
    else
        ReleaseMutex(playerMutex);
    ReleaseMutex(serviceMutex);
    CloseHandle(serviceMutex);
    QCoreApplication::quit();
}

int main(int argc, char **argv)
{
    DDSvc ddsvc(argc, argv);
    return ddsvc.exec();
}
