#pragma once

#include <QtCore>

#include <Windows.h>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QFileInfo)
QT_END_NAMESPACE

namespace Utils
{

void fileLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
QStringList externalFilesToLoad(const QFileInfo &originalMediaFile, const QString &fileType);
void moveToCenter(QObject *window);
int Exit(int resultCode = 0, bool trulyExit = false, HANDLE mutex = nullptr, HWND wallpaper = nullptr);
bool run(const QString &path, const QStringList &params = QStringList{}, bool needAdmin = false, bool needHide = false);
bool isVideo(const QString &fileName);
bool isAudio(const QString &fileName);
bool isPicture(const QString &fileName);
bool installTranslation(const QString &language, const QString &prefix);

}
