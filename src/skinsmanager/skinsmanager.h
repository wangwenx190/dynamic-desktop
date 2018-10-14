#pragma once

#include "skinsmanager_global.h"

#include <QtCore>

class SKINSMANAGERSHARED_EXPORT SkinsManager
{
public:
    static SkinsManager *getInstance();

public:
    QString currentSkinName() const;
    QString currentSkinPath() const;
    QString currentSkinContent() const;
    bool setSkin(const QString &skin = QStringLiteral("Default"));

private:
    QString curSkinName;
    QString curSkinPath;
};
