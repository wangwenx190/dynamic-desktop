#include "skinmanager.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QApplication>

SkinManager *SkinManager::getInstance()
{
    static SkinManager skinManager;
    return &skinManager;
}

QString SkinManager::currentSkinName() const
{
    return curSkinName;
}

QString SkinManager::currentSkinPath() const
{
    return curSkinPath;
}

QString SkinManager::currentSkinContent() const
{
    return qApp->styleSheet();
}

bool SkinManager::setSkin(const QString &skin)
{
    if (skin.isEmpty())
        return false;
    if (skin.toLower() == QStringLiteral("none"))
    {
        qApp->setStyleSheet(QString());
        return true;
    }
    QString filePath;
    if (skin.contains(QStringLiteral("/")) || skin.contains(QStringLiteral("\\")))
        filePath = skin;
    else
#ifndef BUILD_DD_STATIC
        filePath = QApplication::applicationDirPath() + QDir::separator() + QStringLiteral("skins")
                + QDir::separator() + skin + QStringLiteral(".css");
#else
        filePath = QStringLiteral(":/skins/") + skin + QStringLiteral(".css");
#endif
    if (!QFileInfo::exists(filePath))
        return false;
    if (!QFileInfo(filePath).isFile())
        return false;
    QFile skinFile(filePath);
    if (skinFile.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream ts(&skinFile);
        QString str = ts.readAll();
        skinFile.close();
        if (str.isEmpty())
            return false;
        qApp->setStyleSheet(str);
        curSkinPath = filePath;
        curSkinName = QFileInfo(curSkinPath).completeBaseName();
        return true;
    }
    return false;
}
