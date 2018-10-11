#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloadmanager.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_check_update, &QPushButton::clicked, this, &MainWindow::requestUpdate);
    connect(ui->pushButton_close, &QPushButton::clicked,
        [=]
        {
            qApp->setQuitOnLastWindowClosed(true);
            this->close();
        });
    connect(this, &MainWindow::requestUpdate,
        [=]
        {
            bool ok = false;
            QString url = QInputDialog::getText(nullptr, QStringLiteral("Input"), QStringLiteral("url"), QLineEdit::Normal, QStringLiteral("https://"), &ok);
            if (ok && !url.isEmpty())
            {
                QString path = QFileDialog::getSaveFileName(nullptr, tr("Save File"), QStringLiteral("."), tr("Archive (*.7z);;All files (*)"));
                if (!path.isEmpty())
                {
                    if (download != nullptr)
                        this->finish(0);
                    download = new DownloadManager(this);
                    connect(download, &DownloadManager::progressChanged, this, &MainWindow::updateProgressbar);
                    connect(download, &DownloadManager::finished, this, &MainWindow::finish);
                    emit download->download(url, path);
                }
            }
        });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete download;
}

void MainWindow::updateProgressbar(qint64 current, qint64 total)
{
    if (ui->progressBar->maximum() != total)
        ui->progressBar->setRange(0, total);
    if (ui->progressBar->value() != current)
        ui->progressBar->setValue(current);
}

void MainWindow::finish(int code)
{
    Q_UNUSED(code)
    disconnect(download, &DownloadManager::progressChanged, this, &MainWindow::updateProgressbar);
    disconnect(download, &DownloadManager::finished, this, &MainWindow::finish);
    delete download;
    download = nullptr;
}
