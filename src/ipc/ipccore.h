#pragma once

#include "rep_ipc_source.h"

#include <QVariant>

class IPCCore : public DDIPCSimpleSource
{
    Q_OBJECT
public:
    explicit IPCCore(QObject *parent = nullptr);
    ~IPCCore() override;

public slots:
    void sendMessageToServer(const QPair<QString, QVariant> &message) override;
};
