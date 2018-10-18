#include <QtService>
#include <Utils>

#include <Windows.h>

#include <QCoreApplication>
#include <QFileInfo>

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
    QString path = QCoreApplication::applicationDirPath() + QStringLiteral("/launcher");
#ifdef _DEBUG
    path += QStringLiteral("d");
#endif
    path += QStringLiteral(".exe");
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
        if (QFileInfo::exists(path))
            Utils::run(path, QStringList() << QStringLiteral("--launch"));
    }
    else
        ReleaseMutex(playerMutex);
    ReleaseMutex(serviceMutex);
    CloseHandle(serviceMutex);
    qApp->quit();
}

int main(int argc, char **argv)
{
    DDSvc svc(argc, argv);
    return svc.exec();
}
