#pragma once

#include <QtCore>

class SkinManager
{
public:
    static SkinManager *getInstance();

public:
    QString currentSkinName() const;
    QString currentSkinPath() const;
    QString currentSkinContent() const;
    bool setSkin(const QString &skin = QStringLiteral("default"));

private:
    QString curSkinName;
    QString curSkinPath;
};
