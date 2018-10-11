#include "downloadmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QFileInfo>
#include <QDebug>
#include <QFile>
#include <QTemporaryDir>
#include <QDir>

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(this, &DownloadManager::download,
        [=](const QString &qurl, const QString &path)
        {
            if (!qurl.isEmpty() && !path.isEmpty())
                if (stopped)
                {
                    if (!dir)
                        dir = new QTemporaryDir();
                    if (dir->isValid())
                    {
                        stopped = false;
                        url = QUrl(qurl);
                        filePath = path;
                        fileName = QFileInfo(filePath).fileName();
                        tempPath = dir->path() + QDir::separator() + fileName;
                        if (bytesCurrentReceived <= 0)
                            if (QFileInfo::exists(tempPath))
                                QFile::remove(tempPath);
                        QNetworkRequest request(url);
                        /*if (redirection)
                        {
                            url = manager->get(request)->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
                            request.setUrl(url);
                        }*/
                        if (breakPoint)
                        {
                            QString range = QStringLiteral("bytes=%0-").arg(bytesCurrentReceived);
                            request.setRawHeader("Range", range.toLatin1());
                        }
                        reply = manager->get(request);
                        connect(reply, &QNetworkReply::downloadProgress, this, &DownloadManager::onProgressChanged);
                        connect(reply, &QNetworkReply::readyRead, this, &DownloadManager::onReadyRead);
                        connect(reply, &QNetworkReply::finished, this, &DownloadManager::onFinished);
                        connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &DownloadManager::onError);
                    }
                }
        });
    connect(this, &DownloadManager::setBreakPointEnabled,
        [=](bool enable)
        {
            if (breakPoint != enable)
                breakPoint = enable;
        });
    connect(this, &DownloadManager::setRedirectionEnabled,
        [=](bool enable)
        {
            if (redirection != enable)
                redirection = enable;
        });
    connect(this, &DownloadManager::pause, this, &DownloadManager::stopDownload);
    connect(this, &DownloadManager::resume,
        [=]
        {
            emit this->download(getUrl(), getFilePath());
        });
    connect(this, &DownloadManager::stop, this, &DownloadManager::closeDownload);
}

DownloadManager::~DownloadManager()
{
    delete manager;
    delete reply;
    delete file;
    delete dir;
}

bool DownloadManager::supportBreakPoint() const
{
    return breakPoint;
}

bool DownloadManager::supportRedirection() const
{
    return redirection;
}

QString DownloadManager::getUrl() const
{
    return url.toString();
}

QString DownloadManager::getFileName() const
{
    return fileName;
}

QString DownloadManager::getFilePath() const
{
    return filePath;
}

void DownloadManager::onProgressChanged(qint64 received, qint64 total)
{
    if (!stopped)
    {
        bytesReceived = received;
        bytesTotal = total;
        emit progressChanged(bytesReceived + bytesCurrentReceived, bytesTotal + bytesCurrentReceived);
    }
}

void DownloadManager::onReadyRead()
{
    if (!stopped && reply)
    {
        if (!dir)
            dir = new QTemporaryDir();
        if (dir->isValid())
        {
            if (!file)
                file = new QFile(tempPath);
            if (file->open(QFile::WriteOnly | QFile::Append))
            {
                file->write(reply->readAll());
                file->close();
            }
        }
    }
}

void DownloadManager::onFinished()
{
    stopped = true;
    if (reply->error() == QNetworkReply::NoError)
    {
        if (QFileInfo::exists(filePath))
            QFile::remove(filePath);
        QFileInfo fi(*file);
        QFile::copy(fi.absoluteFilePath(), filePath);
        QFile::remove(fi.absoluteFilePath());
        delete file;
        file = nullptr;
        delete dir;
        dir = nullptr;
    }
    else
        qDebug() << reply->errorString();
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    emit finished(statusCode.toInt());
}

void DownloadManager::onError(QNetworkReply::NetworkError code)
{
    Q_UNUSED(code)
    const QString errorText = reply->errorString();
    qDebug() << errorText;
    closeDownload();
    emit error(errorText);
}

void DownloadManager::stopWork()
{
    stopped = true;
    if (reply != nullptr)
    {
        disconnect(reply, &QNetworkReply::downloadProgress, this, &DownloadManager::onProgressChanged);
        disconnect(reply, &QNetworkReply::readyRead, this, &DownloadManager::onReadyRead);
        disconnect(reply, &QNetworkReply::finished, this, &DownloadManager::onFinished);
        disconnect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &DownloadManager::onError);
        reply->abort();
        reply->deleteLater();
        reply = nullptr;
    }
}

void DownloadManager::reset()
{
    bytesCurrentReceived = 0;
    bytesReceived = 0;
    bytesTotal = 0;
}

void DownloadManager::closeDownload()
{
    stopWork();
    reset();
    if (QFileInfo::exists(tempPath))
        QFile::remove(tempPath);
    delete file;
    file = nullptr;
    delete dir;
    dir = nullptr;
}

void DownloadManager::stopDownload()
{
    if (!stopped)
    {
        bytesCurrentReceived += bytesReceived;
        stopWork();
    }
}
