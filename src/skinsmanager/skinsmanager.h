#pragma once

#include "../dd_dll_global.h"

#include <QtCore>

class DD_SHARED_EXPORT SkinsManager
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
