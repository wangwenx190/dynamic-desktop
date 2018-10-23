#pragma once

#include "../dd_dll_global.h"

#include <QObject>
#include <QVariant>

class DD_SHARED_EXPORT IPCServer : public QObject
{
    Q_OBJECT
public:
    static IPCServer *getInstance();

private:
    explicit IPCServer(QObject *parent = nullptr);
    ~IPCServer() override;

signals:
    void clientMessage(QPair<QString, QVariant> message);
    void serverMessage(QPair<QString, QVariant> message);
    void clientOnline();
    void clientOffline();
};
