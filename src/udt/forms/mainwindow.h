#pragma once

#include <QWidget>

namespace Ui
{
    class MainWindow;
}

QT_FORWARD_DECLARE_CLASS(DownloadManager)

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void requestUpdate();

private slots:
    //void init();
    //bool checkUpdate();
    //bool downloadFile();
    void updateProgressbar(qint64 current, qint64 total);
    void finish(int code);

private:
    Ui::MainWindow *ui;
    DownloadManager *download = nullptr;
};
