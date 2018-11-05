#pragma once

#include <QtCore>

QT_FORWARD_DECLARE_CLASS(QFileInfo)

namespace Utils
{

void fileLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
QStringList externalFilesToLoad(const QFileInfo &originalMediaFile, const QString &fileType);
void moveToCenter(QObject *window);
bool run(const QString &path, const QStringList &params = QStringList{}, bool needAdmin = false, bool needHide = false);
bool isVideo(const QString &fileName);
bool isAudio(const QString &fileName);
bool isPicture(const QString &fileName);
bool installTranslation(const QString &language, const QString &prefix);

}
