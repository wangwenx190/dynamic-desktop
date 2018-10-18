#pragma once

#include "../dd_dll_global.h"

#include <Windows.h>

#include <QtCore>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QFileInfo)
QT_END_NAMESPACE

namespace Utils
{

DD_SHARED_EXPORT void fileLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
DD_SHARED_EXPORT QStringList externalFilesToLoad(const QFileInfo &originalMediaFile, const QString &fileType);
DD_SHARED_EXPORT void moveToCenter(QObject *window);
DD_SHARED_EXPORT bool adminRun(const QString &path, const QString &params);
DD_SHARED_EXPORT bool checkUpdate(bool autoUpdate = true);
DD_SHARED_EXPORT bool launchSession1Process(const QString &path, const QString &params);
DD_SHARED_EXPORT int Exit(int resultCode = 0, bool trulyExit = true, HANDLE mutex = nullptr, HWND workerw = nullptr);
DD_SHARED_EXPORT bool run(const QString &path, const QStringList &params = QStringList{}, bool needAdmin = false);
DD_SHARED_EXPORT bool killProcess(const QString &name);

}
