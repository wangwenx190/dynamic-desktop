#pragma once

#include "../dd_dll_global.h"

#include <QObject>
#include <QVariant>

class DD_SHARED_EXPORT IPCClient : public QObject
{
    Q_OBJECT
public:
    explicit IPCClient(QObject *parent = nullptr);
    ~IPCClient() override;

signals:
    void serverMessage(QPair<QString, QVariant>);
    void clientMessage(QPair<QString, QVariant>);
};
