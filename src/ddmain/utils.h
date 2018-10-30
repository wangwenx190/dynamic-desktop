#pragma once

#include <Windows.h>

#include <QtCore>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QFileInfo)
QT_END_NAMESPACE

namespace Utils
{

void fileLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
QStringList externalFilesToLoad(const QFileInfo &originalMediaFile, const QString &fileType);
void moveToCenter(QObject *window);
bool adminRun(const QString &path, const QString &params);
bool launchSession1Process(const QString &path, const QString &params);
int Exit(int resultCode = 0, bool trulyExit = false, HANDLE mutex = nullptr, HWND wallpaper = nullptr);
bool run(const QString &path, const QStringList &params = QStringList{}, bool needAdmin = false);
//bool killProcess(const QString &name);
bool isVideo(const QString &fileName);
bool isAudio(const QString &fileName);
bool isPicture(const QString &fileName);
bool installTranslation(const QString &language, const QString &prefix);
bool isAutoStart();
bool setAutoStart(bool autoStart = true);

}
