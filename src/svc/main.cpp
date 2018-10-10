#include "qtservice.h"

#include <Windows.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QProcess>
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
    setServiceFlags(QtServiceBase::CanBeSuspended);
    setStartupType(QtServiceController::AutoStartup);
}

void DDSvc::start()
{
    QString dir = QCoreApplication::applicationDirPath();
    QString path = dir + QStringLiteral("/dd");
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
        return;
    }
    ReleaseMutex(mutex);
    CloseHandle(mutex);
    if (QFileInfo::exists(path))
        QProcess::startDetached(QDir::toNativeSeparators(path), QStringList{}, QDir::toNativeSeparators(dir));
}

int main(int argc, char **argv)
{
    DDSvc svc(argc, argv);
    return svc.exec();
}
