#include "ipcclient.h"

#include "rep_ipc_replica.h"

QRemoteObjectNode *clientNode = nullptr;
DDIPCReplica *ipcClient = nullptr;

IPCClient *IPCClient::getInstance()
{
    static IPCClient client;
    return &client;
}

IPCClient::IPCClient(QObject *parent) : QObject(parent)
{
    clientNode = new QRemoteObjectNode();
    clientNode->connectToNode(QUrl(QStringLiteral("local:dynamicdesktop")));
    ipcClient = clientNode->acquire<DDIPCReplica>();
    connect(ipcClient, &DDIPCReplica::serverMessage, this, &IPCClient::serverMessage);
    connect(this, &IPCClient::clientMessage, ipcClient, &DDIPCReplica::sendMessageToServer);
    connect(ipcClient, &DDIPCReplica::stateChanged, this, [=](QRemoteObjectReplica::State state, QRemoteObjectReplica::State oldState)
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
    delete clientNode;
}
