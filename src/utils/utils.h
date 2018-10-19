#pragma once

#include "../dd_dll_global.h"

#include <Windows.h>

#include <QtCore>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QFileInfo)
QT_END_NAMESPACE

namespace Utils
{

void DD_SHARED_EXPORT fileLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
QStringList DD_SHARED_EXPORT externalFilesToLoad(const QFileInfo &originalMediaFile, const QString &fileType);
void DD_SHARED_EXPORT moveToCenter(QObject *window);
bool DD_SHARED_EXPORT adminRun(const QString &path, const QString &params);
bool DD_SHARED_EXPORT checkUpdate(bool autoUpdate = true);
bool DD_SHARED_EXPORT launchSession1Process(const QString &path, const QString &params);
int DD_SHARED_EXPORT Exit(int resultCode = 0, bool trulyExit = true, HANDLE mutex = nullptr, HWND workerw = nullptr);
bool DD_SHARED_EXPORT run(const QString &path, const QStringList &params = QStringList{}, bool needAdmin = false);
bool DD_SHARED_EXPORT killProcess(const QString &name);

}
