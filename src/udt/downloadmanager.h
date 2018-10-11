#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QNetworkAccessManager)
QT_FORWARD_DECLARE_CLASS(QFile)
QT_FORWARD_DECLARE_CLASS(QTemporaryDir)
QT_END_NAMESPACE

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    DownloadManager(QObject *parent = nullptr);
    ~DownloadManager() override;

signals:
    void progressChanged(qint64, qint64);
    void finished(int);
    void error(const QString &);

signals:
    void setBreakPointEnabled(bool);
    void setRedirectionEnabled(bool);
    void download(const QString &, const QString &);
    void pause();
    void resume();
    void stop();

public:
    bool supportBreakPoint() const;
    bool supportRedirection() const;
    QString getUrl() const;
    QString getFileName() const;
    QString getFilePath() const;

private slots:
    void onProgressChanged(qint64 received, qint64 total);
    void onReadyRead();
    void onFinished();
    void onError(QNetworkReply::NetworkError code);
    void stopWork();
    void reset();
    void closeDownload();
    void stopDownload();

private:
    QNetworkAccessManager *manager = nullptr;
    QNetworkReply *reply = nullptr;
    QFile *file = nullptr;
    QTemporaryDir *dir = nullptr;
    QUrl url;
    QString filePath, fileName, tempPath;
    bool breakPoint = true, redirection = false, stopped = true;
    qint64 bytesReceived = 0, bytesTotal = 0, bytesCurrentReceived = 0;
};
