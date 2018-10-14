#pragma once

#include "utils_global.h"

#include <Windows.h>

#include <QtCore>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QFileInfo)
QT_END_NAMESPACE

namespace Utils
{

UTILSSHARED_EXPORT HANDLE getAppMutex();
UTILSSHARED_EXPORT void setAppMutex(HANDLE appMutex);
UTILSSHARED_EXPORT void preExit();
UTILSSHARED_EXPORT void Exit(int resultCode = 0);
UTILSSHARED_EXPORT int ExitProgram(int resultCode = 0);
UTILSSHARED_EXPORT void fileLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
UTILSSHARED_EXPORT QStringList externalFilesToLoad(const QFileInfo &originalMediaFile, const QString &fileType);
UTILSSHARED_EXPORT void moveToCenter(QObject *window);
UTILSSHARED_EXPORT bool adminRun(const QString &path, const QString &params);
UTILSSHARED_EXPORT bool checkUpdate(bool hide = true);
UTILSSHARED_EXPORT bool launchSession1Process(const QString &path, const QString &params);
UTILSSHARED_EXPORT bool setAutoStart(bool enable = true);
UTILSSHARED_EXPORT bool isAutoStart(const QString &name = QStringLiteral("Dynamic Desktop Service"));

}
