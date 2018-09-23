#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "settingsmanager.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    setFixedHeight(height());
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
            ui->checkBox_hwdec_cuda->setEnabled(hwdecEnabled);
            ui->checkBox_hwdec_d3d11->setEnabled(hwdecEnabled);
            ui->checkBox_hwdec_dxva->setEnabled(hwdecEnabled);
            if (hwdecEnabled && this->isVisible() && this->isActiveWindow())
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
    QWidget::showEvent(event);
    refreshUI();
}

void PreferencesDialog::refreshUI()
{
    ui->lineEdit_url->setText(SettingsManager::getInstance()->getUrl());
    ui->checkBox_volume->setChecked(!SettingsManager::getInstance()->getMute());
    ui->horizontalSlider_volume->setEnabled(ui->checkBox_volume->isChecked());
    ui->horizontalSlider_volume->setValue(SettingsManager::getInstance()->getVolume());
    ui->checkBox_autostart->setChecked(SettingsManager::getInstance()->getAutostart() && SettingsManager::getInstance()->isRegAutostart());
    QStringList decoders = SettingsManager::getInstance()->getDecoders();
    ui->checkBox_hwdec_cuda->setChecked(decoders.contains(QStringLiteral("CUDA")));
    ui->checkBox_hwdec_d3d11->setChecked(decoders.contains(QStringLiteral("D3D11")));
    ui->checkBox_hwdec_dxva->setChecked(decoders.contains(QStringLiteral("DXVA")));
    bool hwdecEnabled = (ui->checkBox_hwdec_cuda->isChecked()
            || ui->checkBox_hwdec_d3d11->isChecked()
            || ui->checkBox_hwdec_dxva->isChecked())
            && SettingsManager::getInstance()->getHwdec();
    ui->checkBox_hwdec->setChecked(hwdecEnabled);
    static bool firstShow = true;
    if (firstShow)
    {
        ui->checkBox_hwdec_cuda->setEnabled(hwdecEnabled);
        ui->checkBox_hwdec_d3d11->setEnabled(hwdecEnabled);
        ui->checkBox_hwdec_dxva->setEnabled(hwdecEnabled);
        firstShow = false;
    }
}

void PreferencesDialog::saveSettings()
{
    if (ui->lineEdit_url->text() != SettingsManager::getInstance()->getUrl())
    {
        SettingsManager::getInstance()->setUrl(ui->lineEdit_url->text());
        emit urlChanged(ui->lineEdit_url->text());
    }
    if (ui->checkBox_volume->isChecked() != !SettingsManager::getInstance()->getMute())
    {
        SettingsManager::getInstance()->setMute(!ui->checkBox_volume->isChecked());
        emit muteChanged(!ui->checkBox_volume->isChecked());
    }
    if (ui->horizontalSlider_volume->value() != SettingsManager::getInstance()->getVolume())
    {
        SettingsManager::getInstance()->setVolume(ui->horizontalSlider_volume->value());
        emit volumeChanged(ui->horizontalSlider_volume->value());
    }
    if (ui->checkBox_autostart->isChecked() != SettingsManager::getInstance()->getAutostart())
    {
        SettingsManager::getInstance()->setAutostart(ui->checkBox_autostart->isChecked());
        emit autostartChanged(ui->checkBox_autostart->isChecked());
    }
    QStringList decoders;
    if (ui->checkBox_hwdec_cuda->isChecked())
        decoders << QStringLiteral("CUDA");
    if (ui->checkBox_hwdec_d3d11->isChecked())
        decoders << QStringLiteral("D3D11");
    if (ui->checkBox_hwdec_dxva->isChecked())
        decoders << QStringLiteral("DXVA");
    if (decoders.isEmpty())
        ui->checkBox_hwdec->setChecked(false);
    if (ui->checkBox_hwdec->isChecked() != SettingsManager::getInstance()->getHwdec())
        SettingsManager::getInstance()->setHwdec(ui->checkBox_hwdec->isChecked());
    decoders << QStringLiteral("FFmpeg");
    if (decoders != SettingsManager::getInstance()->getDecoders())
        SettingsManager::getInstance()->setDecoders(decoders);
}
