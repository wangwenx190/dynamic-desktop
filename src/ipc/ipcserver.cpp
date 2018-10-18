#include "ipcserver.h"

#include "ipccore.h"

IPCCore *ipcSource = nullptr;
QRemoteObjectHost *srcNode = nullptr;

IPCServer::IPCServer(QObject *parent) : QObject(parent)
{
    ipcSource = new IPCCore();
    srcNode = new QRemoteObjectHost(QUrl(QStringLiteral("local:replica")));
    srcNode->enableRemoting(ipcSource);
    connect(ipcSource, &DDIPCSimpleSource::clientMessage, this, &IPCServer::clientMessage);
    connect(this, &IPCServer::serverMessage, ipcSource, &DDIPCSimpleSource::serverMessage);
}

IPCServer::~IPCServer()
{
    delete srcNode;
    delete ipcSource;
}
