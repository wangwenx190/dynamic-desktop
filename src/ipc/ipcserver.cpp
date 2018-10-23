#include "ipcserver.h"

#include "ipccore.h"

IPCCore *ipcServer = nullptr;
QRemoteObjectHost *serverNode = nullptr;

IPCServer *IPCServer::getInstance()
{
    static IPCServer server;
    return &server;
}

IPCServer::IPCServer(QObject *parent) : QObject(parent)
{
    ipcServer = new IPCCore();
    serverNode = new QRemoteObjectHost(QUrl(QStringLiteral("local:dynamicdesktop")));
    serverNode->enableRemoting(ipcServer);
    connect(ipcServer, &DDIPCSimpleSource::clientMessage, this, [=](const QPair<QString, QVariant> &message)
    {
        if (message.first == QStringLiteral("clientOnline"))
            emit this->clientOnline();
        else if (message.first == QStringLiteral("clientOffline"))
            emit this->clientOffline();
        else
            emit this->clientMessage(message);
    });
    connect(this, &IPCServer::serverMessage, ipcServer, &DDIPCSimpleSource::serverMessage);
}

IPCServer::~IPCServer()
{
    delete serverNode;
    delete ipcServer;
}
