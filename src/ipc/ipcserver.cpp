#include "ipcserver.h"

#include "ipccore.h"

IPCCore *ipcServer = nullptr;
QRemoteObjectHost *serverNode = nullptr;

IPCServer::IPCServer(QObject *parent) : QObject(parent)
{
    ipcServer = new IPCCore();
    serverNode = new QRemoteObjectHost(QUrl(QStringLiteral("local:dynamicdesktop")));
    serverNode->enableRemoting(ipcServer);
    connect(ipcServer, &DDIPCSimpleSource::clientMessage, this, &IPCServer::clientMessage);
    connect(this, &IPCServer::serverMessage, ipcServer, &DDIPCSimpleSource::serverMessage);
}

IPCServer::~IPCServer()
{
    delete serverNode;
    delete ipcServer;
}
