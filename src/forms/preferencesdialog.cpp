#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include "settingsmanager.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QIcon>
#include <QDragEnterEvent>
#include <QUrl>
#include <QMimeDatabase>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    FramelessWindow(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    setContentsMargins(0, 0, 0, 0);
    setResizeable(true);
    setResizeableAreaWidth(5);
    setTitleBar(ui->widget_windowTitleBar);
    addIgnoreWidget(ui->label_windowTitle);
    connect(ui->pushButton_about, SIGNAL(clicked()), this, SIGNAL(about()));
    connect(ui->checkBox_localize, &QCheckBox::stateChanged,
        [=]
        {
            if (this->isVisible() && this->isActiveWindow())
                QMessageBox::information(nullptr, QStringLiteral("Dynamic Desktop"), tr("Application restart is needed to apply this change."));
        });
    connect(ui->pushButton_minimize, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(ui->pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pushButton_maximize, &QPushButton::clicked,
        [=]
        {
            if (this->isMaximized())
                this->showNormal();
            else
                this->showMaximized();
        });
    connect(ui->checkBox_volume, &QCheckBox::stateChanged,
        [=]
        {
            ui->horizontalSlider_volume->setEnabled(ui->checkBox_volume->isChecked());
        });
    connect(ui->pushButton_play, &QPushButton::clicked,
        [=]
        {
            if (ui->lineEdit_url->text() != SettingsManager::getInstance()->getUrl())
                emit this->urlChanged(ui->lineEdit_url->text());
            else
                emit this->play();
        });
    connect(ui->pushButton_pause, SIGNAL(clicked()), this, SIGNAL(pause()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pushButton_ok, &QPushButton::clicked,
        [=]
        {
            closing = true;
            this->saveSettings();
            this->close();
        });
    connect(ui->pushButton_url_browse, &QPushButton::clicked,
        [=]
        {
            QString lastDir = SettingsManager::getInstance()->getUrl();
            lastDir = lastDir.isEmpty() ? QStringLiteral(".") : QFileInfo(lastDir).dir().absolutePath();
            QString path = QFileDialog::getOpenFileName(nullptr, tr("Please select a media file"), lastDir, tr("Videos (*.avi *.mp4 *.mkv *.flv);;All files (*)"));
            if (!path.isEmpty())
                ui->lineEdit_url->setText(QDir::toNativeSeparators(path));
        });
    connect(ui->pushButton_url_input, &QPushButton::clicked,
        [=]
        {
            bool ok;
            QString url = QInputDialog::getText(nullptr, tr("Please input a valid url"), tr("URL"), QLineEdit::Normal, QStringLiteral("https://"), &ok);
            if (ok && !url.isEmpty())
                ui->lineEdit_url->setText(url);
        });
    connect(ui->checkBox_hwdec, &QCheckBox::stateChanged,
        [=]
        {
            bool hwdecEnabled = ui->checkBox_hwdec->isChecked();
            /*if (!SettingsManager::getInstance()->hasNvidiaCard())
            {
                ui->checkBox_hwdec_cuda->setChecked(false);
                ui->checkBox_hwdec_cuda->setEnabled(false);
            }
            else*/
                ui->checkBox_hwdec_cuda->setEnabled(hwdecEnabled);
            /*if (!SettingsManager::getInstance()->hasNvidiaCard()
                    && !SettingsManager::getInstance()->hasAmdCard()
                    && !SettingsManager::getInstance()->hasIntelCard())
            {
                ui->checkBox_hwdec_d3d11->setChecked(false);
                ui->checkBox_hwdec_d3d11->setEnabled(false);
                ui->checkBox_hwdec_dxva->setChecked(false);
                ui->checkBox_hwdec_dxva->setEnabled(false);
            }
            else*/
            //{
                ui->checkBox_hwdec_d3d11->setEnabled(hwdecEnabled);
                ui->checkBox_hwdec_dxva->setEnabled(hwdecEnabled);
            //}
            if (this->isVisible() && this->isActiveWindow())
                QMessageBox::information(nullptr, QStringLiteral("Dynamic Desktop"), tr("Restart this application to experience it.\nMake sure this application runs in your GPU's Optimus mode."));
        });
    connect(this, SIGNAL(refreshUi()), this, SLOT(refreshUI()));
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::setAudioAreaEnabled(bool enabled)
{
    ui->checkBox_volume->setEnabled(enabled);
    ui->horizontalSlider_volume->setEnabled(enabled && ui->checkBox_volume->isChecked() ? true : false);
}

void PreferencesDialog::showEvent(QShowEvent *event)
{
    FramelessWindow::showEvent(event);
    closing = false;
    refreshUI();
}

void PreferencesDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange)
        if (windowState() == Qt::WindowMaximized)
            ui->pushButton_maximize->setIcon(QIcon(QStringLiteral(":/restore.svg")));
        else
            ui->pushButton_maximize->setIcon(QIcon(QStringLiteral(":/maximize.svg")));
    FramelessWindow::changeEvent(event);
}

static bool canHandleDrop(const QDragEnterEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.empty()) return false;
    QMimeDatabase mimeDatabase;
    return SettingsManager::getInstance()->supportedMimeTypes().
        contains(mimeDatabase.mimeTypeForUrl(urls.constFirst()).name());
}

void PreferencesDialog::dragEnterEvent(QDragEnterEvent *event)
{
    event->setAccepted(canHandleDrop(event));
    FramelessWindow::dragEnterEvent(event);
}

void PreferencesDialog::dropEvent(QDropEvent *event)
{
    event->accept();
    QUrl url = event->mimeData()->urls().constFirst();
    QString path;
    if (url.isLocalFile())
        path = QDir::toNativeSeparators(url.toLocalFile());
    else
        path = url.url();
    ui->lineEdit_url->setText(path);
    FramelessWindow::dropEvent(event);
}

void PreferencesDialog::refreshUI()
{
    if (closing)
        return;
    ui->lineEdit_url->setText(SettingsManager::getInstance()->getUrl());
    ui->checkBox_volume->setChecked(!SettingsManager::getInstance()->getMute());
    ui->horizontalSlider_volume->setEnabled(ui->checkBox_volume->isChecked());
    ui->horizontalSlider_volume->setValue(SettingsManager::getInstance()->getVolume());
    ui->checkBox_autoStart->setChecked(SettingsManager::getInstance()->getAutostart());
    QStringList decoders = SettingsManager::getInstance()->getDecoders();
    ui->checkBox_hwdec_cuda->setChecked(decoders.contains(QStringLiteral("CUDA"))
                                        /*&& SettingsManager::getInstance()->hasNvidiaCard()*/);
    ui->checkBox_hwdec_d3d11->setChecked(decoders.contains(QStringLiteral("D3D11"))
                                         /*&& (SettingsManager::getInstance()->hasNvidiaCard()
                                             || SettingsManager::getInstance()->hasAmdCard()
                                             || SettingsManager::getInstance()->hasIntelCard())*/);
    ui->checkBox_hwdec_dxva->setChecked(decoders.contains(QStringLiteral("DXVA"))
                                        /*&& (SettingsManager::getInstance()->hasNvidiaCard()
                                            || SettingsManager::getInstance()->hasAmdCard()
                                            || SettingsManager::getInstance()->hasIntelCard())*/);
    bool hwdecEnabled = (ui->checkBox_hwdec_cuda->isChecked()
            || ui->checkBox_hwdec_d3d11->isChecked()
            || ui->checkBox_hwdec_dxva->isChecked())
            && SettingsManager::getInstance()->getHwdec();
    ui->checkBox_hwdec->setChecked(hwdecEnabled);
    static bool firstShow = true;
    if (firstShow)
    {
        ui->checkBox_hwdec_cuda->setEnabled(hwdecEnabled
                               /*&& SettingsManager::getInstance()->hasNvidiaCard()*/);
        ui->checkBox_hwdec_d3d11->setEnabled(hwdecEnabled
                               /*&& (SettingsManager::getInstance()->hasNvidiaCard()
                                    || SettingsManager::getInstance()->hasAmdCard()
                                    || SettingsManager::getInstance()->hasIntelCard())*/);
        ui->checkBox_hwdec_dxva->setEnabled(hwdecEnabled
                               /*&& (SettingsManager::getInstance()->hasNvidiaCard()
                                    || SettingsManager::getInstance()->hasAmdCard()
                                    || SettingsManager::getInstance()->hasIntelCard())*/);
        firstShow = false;
    }
    ui->checkBox_localize->setChecked(SettingsManager::getInstance()->getLocalize());
}

void PreferencesDialog::saveSettings()
{
    if (ui->lineEdit_url->text() != SettingsManager::getInstance()->getUrl())
    {
        SettingsManager::getInstance()->setUrl(ui->lineEdit_url->text());
        emit urlChanged(SettingsManager::getInstance()->getUrl());
    }
    if (ui->checkBox_volume->isChecked() != !SettingsManager::getInstance()->getMute())
    {
        SettingsManager::getInstance()->setMute(!ui->checkBox_volume->isChecked());
        emit muteChanged(SettingsManager::getInstance()->getMute());
    }
    if (ui->horizontalSlider_volume->value() != SettingsManager::getInstance()->getVolume())
    {
        SettingsManager::getInstance()->setVolume(ui->horizontalSlider_volume->value());
        emit volumeChanged(SettingsManager::getInstance()->getVolume());
    }
    SettingsManager::getInstance()->setAutostart(ui->checkBox_autoStart->isChecked());
    emit autostartChanged(SettingsManager::getInstance()->getAutostart());
    QStringList decoders;
    if (ui->checkBox_hwdec_cuda->isChecked()
            /*&& SettingsManager::getInstance()->hasNvidiaCard()*/)
        decoders << QStringLiteral("CUDA");
    if (ui->checkBox_hwdec_d3d11->isChecked()
            /*&& (SettingsManager::getInstance()->hasNvidiaCard()
                || SettingsManager::getInstance()->hasAmdCard()
                || SettingsManager::getInstance()->hasIntelCard())*/)
        decoders << QStringLiteral("D3D11");
    if (ui->checkBox_hwdec_dxva->isChecked()
            /*&& (SettingsManager::getInstance()->hasNvidiaCard()
                || SettingsManager::getInstance()->hasAmdCard()
                || SettingsManager::getInstance()->hasIntelCard())*/)
        decoders << QStringLiteral("DXVA");
    if (decoders.isEmpty())
        ui->checkBox_hwdec->setChecked(false);
    if (ui->checkBox_hwdec->isChecked() != SettingsManager::getInstance()->getHwdec())
        SettingsManager::getInstance()->setHwdec(ui->checkBox_hwdec->isChecked());
    decoders << QStringLiteral("FFmpeg");
    if (decoders != SettingsManager::getInstance()->getDecoders())
        SettingsManager::getInstance()->setDecoders(decoders);
    if (ui->checkBox_localize->isChecked() != SettingsManager::getInstance()->getLocalize())
        SettingsManager::getInstance()->setLocalize(ui->checkBox_localize->isChecked());
}
