#pragma once

#include "../dd_dll_global.h"

#include <QObject>
#include <QVariant>

class DD_SHARED_EXPORT IPCClient : public QObject
{
    Q_OBJECT
public:
    static IPCClient *getInstance();

private:
    explicit IPCClient(QObject *parent = nullptr);
    ~IPCClient() override;

signals:
    void serverMessage(QPair<QString, QVariant> message);
    void clientMessage(QPair<QString, QVariant> message);
    void serverOnline();
    void serverOffline();
};
