#include "skinsmanager.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QApplication>

SkinsManager *SkinsManager::getInstance()
{
    static SkinsManager skinsManager;
    return &skinsManager;
}

QString SkinsManager::currentSkinName() const
{
    return curSkinName;
}

QString SkinsManager::currentSkinPath() const
{
    return curSkinPath;
}

QString SkinsManager::currentSkinContent() const
{
    return qApp->styleSheet();
}

bool SkinsManager::setSkin(const QString &skin)
{
    if (skin.isEmpty())
        return false;
    if (skin.toLower() == QStringLiteral("none"))
    {
        qApp->setStyleSheet(QString());
        curSkinPath = QString();
        curSkinName = QString();
        return true;
    }
    QString filePath;
    if (skin.contains(QStringLiteral("/")) || skin.contains(QStringLiteral("\\")))
        filePath = skin;
    else
        filePath = QStringLiteral(":/skins/") + skin + QStringLiteral(".css");
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
