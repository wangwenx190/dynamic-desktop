#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "settingsmanager.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    connect(ui->checkBox_mute, &QCheckBox::stateChanged,
        [=]
        {
            ui->horizontalSlider_volume->setEnabled(!ui->checkBox_mute->isChecked());
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
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::setAudioAreaEnabled(bool enabled)
{
    ui->checkBox_mute->setEnabled(enabled);
    if (ui->checkBox_mute->isEnabled())
        ui->horizontalSlider_volume->setEnabled(!enabled);
    else
        ui->horizontalSlider_volume->setEnabled(enabled);
}

void PreferencesDialog::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    refreshUI();
}

void PreferencesDialog::refreshUI()
{
    ui->lineEdit_url->setText(SettingsManager::getInstance()->getUrl());
    ui->checkBox_mute->setChecked(SettingsManager::getInstance()->getMute());
    ui->horizontalSlider_volume->setEnabled(!ui->checkBox_mute->isChecked());
    ui->horizontalSlider_volume->setValue(static_cast<int>(SettingsManager::getInstance()->getVolume() * 10));
    ui->checkBox_autostart->setChecked(SettingsManager::getInstance()->getAutostart());
}

void PreferencesDialog::saveSettings()
{
    if (ui->lineEdit_url->text() != SettingsManager::getInstance()->getUrl())
    {
        SettingsManager::getInstance()->setUrl(ui->lineEdit_url->text());
        emit urlChanged(ui->lineEdit_url->text());
    }
    if (ui->checkBox_mute->isChecked() != SettingsManager::getInstance()->getMute())
    {
        SettingsManager::getInstance()->setMute(ui->checkBox_mute->isChecked());
        emit muteChanged(ui->checkBox_mute->isChecked());
    }
    if (static_cast<qreal>(ui->horizontalSlider_volume->value()) != SettingsManager::getInstance()->getVolume())
    {
        SettingsManager::getInstance()->setVolume(static_cast<qreal>(ui->horizontalSlider_volume->value() / 10));
        emit volumeChanged(static_cast<qreal>(ui->horizontalSlider_volume->value()));
    }
    if (ui->checkBox_autostart->isChecked() != SettingsManager::getInstance()->getAutostart())
    {
        SettingsManager::getInstance()->setAutostart(ui->checkBox_autostart->isChecked());
        emit autostartChanged(ui->checkBox_autostart->isChecked());
    }
}
