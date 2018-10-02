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
#include <QMimeData>
#include <QTextCodec>

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
    connect(ui->pushButton_audio_open, &QPushButton::clicked,
        [=]
        {
            QString audioPath = QFileDialog::getOpenFileName(nullptr, tr("Please select an audio file"), SettingsManager::getInstance()->lastDir(), tr("Audios (*.mka *.aac *.flac *.mp3 *.wav);;All files (*)"));
            if (!audioPath.isEmpty())
                emit this->audioOpened(audioPath);
        });
    connect(ui->pushButton_subtitle_open, &QPushButton::clicked,
        [=]
        {
            QString subtitlePath = QFileDialog::getOpenFileName(nullptr, tr("Please select a subtitle file"), SettingsManager::getInstance()->lastDir(), tr("Subtitles (*.ass *.ssa *.srt *.sup);;All files (*)"));
            if (!subtitlePath.isEmpty())
                emit this->subtitleOpened(subtitlePath);
        });
    connect(this, &PreferencesDialog::clearAllTracks,
        [=]
        {
            ui->comboBox_video_track->clear();
            ui->comboBox_audio_track->clear();
            ui->comboBox_subtitle_track->clear();
        });
    connect(this, &PreferencesDialog::updateVideoTracks,
        [=](const QVariantList &videoTracks)
        {
            if (!videoTracks.isEmpty())
            {
                ui->comboBox_video_track->clear();
                for (auto& track : videoTracks)
                {
                    QVariantMap trackData = track.toMap();
                    unsigned int id = trackData[QStringLiteral("id")].toUInt();
                    QString lang = trackData[QStringLiteral("language")].toString();
                    QString title = trackData[QStringLiteral("title")].toString();
                    QString txt = tr("ID: %0 | Title: %1 | Language: %2")
                            .arg(id).arg(title).arg(lang);
                    ui->comboBox_video_track->addItem(txt, id);
                }
            }
        });
    connect(this, &PreferencesDialog::updateAudioTracks,
        [=](const QVariantList &audioTracks, bool add)
        {
            if (!audioTracks.isEmpty())
            {
                if (!add)
                    ui->comboBox_audio_track->clear();
                for (auto& track : audioTracks)
                {
                    QVariantMap trackData = track.toMap();
                    unsigned int id = trackData[QStringLiteral("id")].toUInt();
                    QString lang = trackData[QStringLiteral("language")].toString();
                    QString title = trackData[QStringLiteral("title")].toString();
                    QString txt = tr("ID: %0 | Title: %1 | Language: %2")
                            .arg(id).arg(title).arg(lang);
                    ui->comboBox_audio_track->addItem(txt, id);
                }
            }
        });
    connect(this, &PreferencesDialog::updateSubtitleTracks,
        [=](const QVariantList &subtitleTracks, bool add)
        {
            if (!subtitleTracks.isEmpty())
            {
                if (!add)
                    ui->comboBox_subtitle_track->clear();
                for (auto& track : subtitleTracks)
                {
                    QVariantMap trackData = track.toMap();
                    if (!add)
                    {
                        unsigned int id = trackData[QStringLiteral("id")].toUInt();
                        QString lang = trackData[QStringLiteral("language")].toString();
                        QString title = trackData[QStringLiteral("title")].toString();
                        QString txt = tr("ID: %0 | Title: %1 | Language: %2")
                                .arg(id).arg(title).arg(lang);
                        ui->comboBox_subtitle_track->addItem(txt, id);
                    }
                    else
                    {
                        QString file = trackData[QStringLiteral("file")].toString();
                        QString txt = tr("File: %0").arg(QFileInfo(file).fileName());
                        ui->comboBox_subtitle_track->addItem(txt, file);
                    }
                }
            }
        });
    connect(this, &PreferencesDialog::updateVolumeArea,
        [=]
        {
            if (audioAvailable)
            {
                ui->checkBox_volume->setChecked(!SettingsManager::getInstance()->getMute());
                ui->horizontalSlider_volume->setEnabled(ui->checkBox_volume->isChecked());
                ui->horizontalSlider_volume->setValue(SettingsManager::getInstance()->getVolume());
            }
        });
    connect(this, &PreferencesDialog::setVolumeAreaEnabled,
        [=](bool enabled)
        {
            ui->checkBox_volume->setEnabled(audioAvailable && enabled);
            ui->horizontalSlider_volume->setEnabled(audioAvailable && enabled && ui->checkBox_volume->isChecked());
        });
    connect(ui->horizontalSlider_volume, &QSlider::sliderMoved,
        [=](int value)
        {
            int vol = value;
            if (vol > 99)
                vol = 99;
            if (vol < 0)
                vol = 0;
            emit this->volumeChanged(static_cast<unsigned int>(vol));
        });
    connect(ui->horizontalSlider_volume, &QSlider::sliderPressed,
        [=]
        {
            int value = ui->horizontalSlider_volume->value();
            if (value > 99)
                value = 99;
            if (value < 0)
                value = 0;
            emit this->volumeChanged(static_cast<unsigned int>(value));
        });
    connect(this, &PreferencesDialog::setAudioAreaEnabled,
        [=](bool audioAvailable)
        {
            this->audioAvailable = audioAvailable;
            ui->groupBox_audio->setEnabled(audioAvailable);
        });
    connect(this, &PreferencesDialog::setSeekAreaEnabled,
        [=](bool enabled)
        {
            ui->horizontalSlider_video_position->setEnabled(enabled);
        });
    connect(ui->horizontalSlider_video_position, &QSlider::sliderMoved,
        [=](int value)
        {
            emit this->seekBySlider(static_cast<qint64>(value * sliderUnit));
        });
    connect(ui->horizontalSlider_video_position, &QSlider::sliderPressed,
        [=]
        {
            emit this->seekBySlider(static_cast<qint64>(ui->horizontalSlider_video_position->value() * sliderUnit));
        });
    connect(this, &PreferencesDialog::updateVideoSliderRange,
        [=](qint64 duration)
        {
            ui->horizontalSlider_video_position->setRange(0, static_cast<int>(duration / sliderUnit));
        });
    connect(this, &PreferencesDialog::updateVideoSliderUnit,
        [=](int unit)
        {
            sliderUnit = unit;
        });
    connect(this, &PreferencesDialog::updateVideoSlider,
        [=](qint64 position)
        {
            ui->horizontalSlider_video_position->setValue(static_cast<int>(position / sliderUnit));
        });
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
                emit this->urlChanged(QString());
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
            QString path = QFileDialog::getOpenFileName(nullptr, tr("Please select a media file"), SettingsManager::getInstance()->lastDir(), tr("Videos (*.avi *.mp4 *.mkv *.flv);;Audios (*.mp3 *.flac *.ape *.wav);;Pictures (*.bmp *.jpg *.jpeg *.png *.gif);;All files (*)"));
            if (!path.isEmpty())
                ui->lineEdit_url->setText(QDir::toNativeSeparators(path));
        });
    connect(ui->pushButton_url_input, &QPushButton::clicked,
        [=]
        {
            bool ok = false;
            QString input = QInputDialog::getText(nullptr, tr("Please input a valid URL"), tr("URL"), QLineEdit::Normal, QStringLiteral("https://"), &ok);
            if (ok && !input.isEmpty())
            {
                QUrl url(input);
                if (url.isValid())
                {
                    if (url.isLocalFile())
                        ui->lineEdit_url->setText(url.toLocalFile());
                    else
                        ui->lineEdit_url->setText(url.url());
                }
                else
                    QMessageBox::warning(nullptr, QStringLiteral("Dynamic Desktop"), tr("\"%0\" is not a valid URL.").arg(input));
            }
        });
    connect(ui->checkBox_hwdec, &QCheckBox::stateChanged,
        [=]
        {
            bool hwdecEnabled = ui->checkBox_hwdec->isChecked();
            ui->checkBox_hwdec_cuda->setEnabled(hwdecEnabled);
            ui->checkBox_hwdec_d3d11->setEnabled(hwdecEnabled);
            ui->checkBox_hwdec_dxva->setEnabled(hwdecEnabled);
            if (this->isVisible() && this->isActiveWindow())
                QMessageBox::information(nullptr, QStringLiteral("Dynamic Desktop"), tr("Reopen this video or play another video to experience it.\nMake sure this application runs in your GPU's Optimus mode."));
        });
    connect(ui->checkBox_hwdec_cuda, &QCheckBox::stateChanged,
        [=]
        {
            if (this->isVisible() && this->isActiveWindow())
                QMessageBox::information(nullptr, QStringLiteral("Dynamic Desktop"), tr("Reopen this video or play another video to experience it.\nMake sure this application runs in your GPU's Optimus mode."));
        });
    connect(ui->checkBox_hwdec_d3d11, &QCheckBox::stateChanged,
        [=]
        {
            if (this->isVisible() && this->isActiveWindow())
                QMessageBox::information(nullptr, QStringLiteral("Dynamic Desktop"), tr("Reopen this video or play another video to experience it.\nMake sure this application runs in your GPU's Optimus mode."));
        });
    connect(ui->checkBox_hwdec_dxva, &QCheckBox::stateChanged,
        [=]
        {
            if (this->isVisible() && this->isActiveWindow())
                QMessageBox::information(nullptr, QStringLiteral("Dynamic Desktop"), tr("Reopen this video or play another video to experience it.\nMake sure this application runs in your GPU's Optimus mode."));
        });
    ui->comboBox_subtitle_charset->addItem(tr("Auto detect"), QStringLiteral("AutoDetect"));
    ui->comboBox_subtitle_charset->addItem(tr("System"), QStringLiteral("System"));
    for (auto& codec : QTextCodec::availableCodecs())
        ui->comboBox_subtitle_charset->addItem(QString::fromLatin1(codec), QString::fromLatin1(codec));
    connect(this, SIGNAL(refreshUi()), this, SLOT(refreshUI()));
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
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
            ui->pushButton_maximize->setIcon(QIcon(QStringLiteral(":/restore.ico")));
        else
            ui->pushButton_maximize->setIcon(QIcon(QStringLiteral(":/maximize.ico")));
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
    if (audioAvailable)
    {
        ui->checkBox_volume->setChecked(!SettingsManager::getInstance()->getMute());
        ui->horizontalSlider_volume->setEnabled(ui->checkBox_volume->isChecked());
        ui->horizontalSlider_volume->setValue(SettingsManager::getInstance()->getVolume());
    }
    ui->checkBox_autoStart->setChecked(SettingsManager::getInstance()->getAutostart());
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
    ui->checkBox_localize->setChecked(SettingsManager::getInstance()->getLocalize());
    ui->radioButton_ratio_fitDesktop->setChecked(SettingsManager::getInstance()->getFitDesktop());
    ui->radioButton_ratio_videoAspectRatio->setChecked(!ui->radioButton_ratio_fitDesktop->isChecked());
    ui->comboBox_subtitle_charset->setCurrentIndex(ui->comboBox_subtitle_charset->findData(SettingsManager::getInstance()->getCharset()));
    ui->checkBox_subtitle_autoLoadExternal->setChecked(SettingsManager::getInstance()->getSubtitleAutoLoad());
    ui->checkBox_displaySubtitle->setChecked(SettingsManager::getInstance()->getSubtitle());
    ui->checkBox_audio_autoLoadExternal->setChecked(SettingsManager::getInstance()->getAudioAutoLoad());
}

void PreferencesDialog::saveSettings()
{
    if (ui->lineEdit_url->text() != SettingsManager::getInstance()->getUrl())
    {
        SettingsManager::getInstance()->setUrl(ui->lineEdit_url->text());
        emit urlChanged(SettingsManager::getInstance()->getUrl());
    }
    else
    {
        emit videoTrackChanged(ui->comboBox_video_track->currentData().toUInt());
        emit audioTrackChanged(ui->comboBox_audio_track->currentData().toUInt());
        emit subtitleTrackChanged(ui->comboBox_subtitle_track->currentData());
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
    if (ui->checkBox_localize->isChecked() != SettingsManager::getInstance()->getLocalize())
        SettingsManager::getInstance()->setLocalize(ui->checkBox_localize->isChecked());
    if (ui->radioButton_ratio_fitDesktop->isChecked() != SettingsManager::getInstance()->getFitDesktop())
    {
        SettingsManager::getInstance()->setFitDesktop(ui->radioButton_ratio_fitDesktop->isChecked());
        emit pictureRatioChanged(SettingsManager::getInstance()->getFitDesktop());
    }
    if (ui->comboBox_subtitle_charset->currentData().toString() != SettingsManager::getInstance()->getCharset())
    {
        SettingsManager::getInstance()->setCharset(ui->comboBox_subtitle_charset->currentData().toString());
        emit charsetChanged(SettingsManager::getInstance()->getCharset());
    }
    if (ui->checkBox_subtitle_autoLoadExternal->isChecked() != SettingsManager::getInstance()->getSubtitleAutoLoad())
    {
        SettingsManager::getInstance()->setSubtitleAutoLoad(ui->checkBox_subtitle_autoLoadExternal->isChecked());
        emit subtitleAutoLoadChanged(SettingsManager::getInstance()->getSubtitleAutoLoad());
    }
    if (ui->checkBox_displaySubtitle->isChecked() != SettingsManager::getInstance()->getSubtitle())
    {
        SettingsManager::getInstance()->setSubtitle(ui->checkBox_displaySubtitle->isChecked());
        emit subtitleEnabled(SettingsManager::getInstance()->getSubtitle());
    }
    SettingsManager::getInstance()->setAudioAutoLoad(ui->checkBox_audio_autoLoadExternal->isChecked());
}
