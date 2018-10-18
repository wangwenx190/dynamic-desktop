#include "ipccore.h"

IPCCore::IPCCore(QObject *parent) : DDIPCSimpleSource(parent)
{
}

void IPCCore::sendMessageToServer(const QPair<QString, QVariant> &message)
{
    emit this->clientMessage(message);
}

IPCCore::~IPCCore() = default;
