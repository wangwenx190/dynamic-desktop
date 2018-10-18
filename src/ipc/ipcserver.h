#pragma once

#include "../dd_dll_global.h"

#include <QObject>
#include <QVariant>

class DD_SHARED_EXPORT IPCServer : public QObject
{
    Q_OBJECT
public:
    explicit IPCServer(QObject *parent = nullptr);
    ~IPCServer() override;

signals:
    void clientMessage(QPair<QString, QVariant>);
    void serverMessage(QPair<QString, QVariant>);
};
