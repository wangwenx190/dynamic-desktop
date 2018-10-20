#include "ipcclient.h"

#include "rep_ipc_replica.h"

QRemoteObjectNode *repNode = nullptr;
DDIPCReplica *ipcReplica = nullptr;

IPCClient::IPCClient(QObject *parent) : QObject(parent)
{
    repNode = new QRemoteObjectNode();
    repNode->connectToNode(QUrl(QStringLiteral("local:replica")));
    ipcReplica = repNode->acquire<DDIPCReplica>();
    connect(ipcReplica, &DDIPCReplica::serverMessage, this, &IPCClient::serverMessage);
    connect(this, &IPCClient::clientMessage, ipcReplica, &DDIPCReplica::sendMessageToServer);
    connect(ipcReplica, &DDIPCReplica::stateChanged, this, [=](QRemoteObjectReplica::State state, QRemoteObjectReplica::State oldState)
    {
        Q_UNUSED(oldState)
        if (state == QRemoteObjectReplica::Valid)
            emit this->serverOnline();
        else if (state == QRemoteObjectReplica::Suspect)
            emit this->serverOffline();
    });
}

IPCClient::~IPCClient()
{
    delete repNode;
}
