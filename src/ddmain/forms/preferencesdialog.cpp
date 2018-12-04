#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "settingsmanager.h"
#ifndef DD_NO_CSS
#include "skinsmanager.h"
#endif
#include "utils.h"
#include <Win32Utils>

#ifndef DD_NO_WIN_EXTRAS
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#endif
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#ifndef DD_NO_DRAG_DROP
#include <QDragEnterEvent>
#endif
#include <QUrl>
#ifndef DD_NO_MIME_TYPE
#include <QMimeDatabase>
#include <QMimeData>
#endif
#include <QTextCodec>
#include <QTimer>
#include <QApplication>
#ifndef BUILD_DD_STATIC
#include <QLibraryInfo>
#endif
#include <QTime>
#include <QComboBox>

#ifndef DD_NO_CSS
void PreferencesDialog::populateSkins(const QString &dirPath, bool add, bool isExternal)
{
    if (dirPath.isEmpty())
        return;
    if (!QFileInfo::exists(dirPath) || !QFileInfo(dirPath).isDir())
        return;
    QDir skinDir(dirPath);
    QFileInfoList skinFileList = skinDir.entryInfoList(QDir::Files, QDir::Name);
    if (skinFileList.count() > 0)
    {
        if (!add)
        {
            ui->comboBox_skin->clear();
            ui->comboBox_skin->addItem(DD_TR("<None>"), QStringLiteral("none"));
        }
        for (auto& skinFile : skinFileList)
            ui->comboBox_skin->addItem(skinFile.completeBaseName(), isExternal ? skinFile.absoluteFilePath() : skinFile.completeBaseName());
    }
    if (ui->comboBox_skin->count() > 1)
        ui->comboBox_skin->setEnabled(true);
    else
        ui->comboBox_skin->setEnabled(false);
}
#endif

#ifndef DD_NO_TRANSLATIONS
void PreferencesDialog::populateLanguages(const QString &dirPath, bool add, bool isExternal)
{
    if (dirPath.isEmpty())
        return;
    if (!QFileInfo::exists(dirPath) || !QFileInfo(dirPath).isDir())
        return;
    QDir languageDir(dirPath);
    QFileInfoList languageFileList = languageDir.entryInfoList(QDir::Files, QDir::Name);
    if (languageFileList.count() > 0)
    {
        if (!add)
        {
            ui->comboBox_language->clear();
            ui->comboBox_language->addItem(DD_TR("Auto"), QStringLiteral("auto"));
            ui->comboBox_language->addItem(QStringLiteral("American English"), QStringLiteral("en"));
        }
        for (auto& languageFile : languageFileList)
        {
            QString fileName = languageFile.completeBaseName();
            if (!fileName.startsWith(QStringLiteral("dd_"), Qt::CaseInsensitive))
                continue;
            QString lang = fileName.mid(fileName.indexOf(QLatin1Char('_')) + 1);
            lang = lang.replace('-', '_');
            QLocale locale(lang);
            ui->comboBox_language->addItem(locale.nativeLanguageName(), isExternal ? languageFile.absoluteFilePath() : lang);
        }
    }
    if (ui->comboBox_language->count() > 2)
        ui->comboBox_language->setEnabled(true);
    else
    {
        ui->comboBox_language->setCurrentIndex(1);
        ui->comboBox_language->setEnabled(false);
    }
}
#endif

void PreferencesDialog::setMediaSliderPosition(qint64 position)
{
    auto value = static_cast<int>(position / sliderUnit);
    if (value != ui->horizontalSlider_video_position->value())
        ui->horizontalSlider_video_position->setValue(value);
}

void PreferencesDialog::setMediaSliderUnit(quint32 unit)
{
    if (sliderUnit != unit)
        sliderUnit = unit;
}

void PreferencesDialog::setMediaSliderRange(qint64 duration)
{
    ui->horizontalSlider_video_position->setRange(0, static_cast<int>(duration / sliderUnit));
    int max = ui->horizontalSlider_video_position->maximum();
    auto singleStep = static_cast<int>(max * 0.01);
    ui->horizontalSlider_video_position->setSingleStep(singleStep);
    auto pageStep = static_cast<int>(max * 0.05);
    ui->horizontalSlider_video_position->setPageStep(pageStep);
}

void PreferencesDialog::setSeekAreaEnabled(bool enabled)
{
    ui->horizontalSlider_video_position->setEnabled(enabled);
}

void PreferencesDialog::setAudioAreaEnabled(bool available)
{
    audioAvailable = available;
    ui->groupBox_audio->setEnabled(audioAvailable);
}

void PreferencesDialog::setVideoTracks(const QVariantList& videoTracks)
{
    if (!videoTracks.isEmpty())
    {
        ui->comboBox_video_track->clear();
        for (auto& track : videoTracks)
        {
            QVariantMap trackData = track.toMap();
            quint32 id = trackData[QStringLiteral("id")].toUInt();
            QString lang = trackData[QStringLiteral("language")].toString();
            QString title = trackData[QStringLiteral("title")].toString();
            QString txt = DD_TR("ID: %0 | Title: %1 | Language: %2")
                    .arg(id).arg(title).arg(lang);
            ui->comboBox_video_track->addItem(txt, id);
        }
    }
    if (ui->comboBox_video_track->count() > 0)
        ui->comboBox_video_track->setEnabled(true);
    else
        ui->comboBox_video_track->setEnabled(false);
}

void PreferencesDialog::setAudioTracks(const QVariantList& audioTracks, bool add)
{
    if (!audioTracks.isEmpty())
    {
        if (!add)
            ui->comboBox_audio_track->clear();
        for (auto& track : audioTracks)
        {
            QVariantMap trackData = track.toMap();
            quint32 id = trackData[QStringLiteral("id")].toUInt();
            QString lang = trackData[QStringLiteral("language")].toString();
            QString title = trackData[QStringLiteral("title")].toString();
            QString txt = DD_TR("ID: %0 | Title: %1 | Language: %2")
                    .arg(id).arg(title).arg(lang);
            ui->comboBox_audio_track->addItem(txt, id);
        }
    }
    if (ui->comboBox_audio_track->count() > 0)
        ui->comboBox_audio_track->setEnabled(true);
    else
        ui->comboBox_audio_track->setEnabled(false);
}

void PreferencesDialog::setSubtitleTracks(const QVariantList& subtitleTracks, bool add)
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
                quint32 id = trackData[QStringLiteral("id")].toUInt();
                QString lang = trackData[QStringLiteral("language")].toString();
                QString title = trackData[QStringLiteral("title")].toString();
                QString txt = DD_TR("ID: %0 | Title: %1 | Language: %2")
                        .arg(id).arg(title).arg(lang);
                ui->comboBox_subtitle_track->addItem(txt, id);
            }
            else
            {
                QString file = trackData[QStringLiteral("file")].toString();
                QString txt = DD_TR("File: %0").arg(QFileInfo(file).fileName());
                ui->comboBox_subtitle_track->addItem(txt, file);
            }
        }
    }
    if (ui->comboBox_subtitle_track->count() > 0)
        ui->comboBox_subtitle_track->setEnabled(true);
    else
        ui->comboBox_subtitle_track->setEnabled(false);
}

void PreferencesDialog::setVideoPositionText(const QString &text)
{
    ui->label_video_position->setText(text);
}

void PreferencesDialog::setVideoDurationText(const QString &text)
{
    ui->label_video_duration->setText(text);
}

void PreferencesDialog::playNextMedia()
{
    moveNextItem(ui->comboBox_url);
}

void PreferencesDialog::playNextPlaylist()
{
    moveNextItem(ui->comboBox_playlists);
}

void PreferencesDialog::playPreviousMedia()
{
    movePreviousItem(ui->comboBox_url);
}

void PreferencesDialog::playPreviousPlaylist()
{
    movePreviousItem(ui->comboBox_playlists);
}

void PreferencesDialog::refreshPlaylistsAndFiles()
{
    if (!refreshingData)
        refreshingData = true;
    populatePlaylists();
    populateFiles();
    if (refreshingData)
        refreshingData = false;
}

void PreferencesDialog::switchPlaylist(const QString &name)
{
    if (name.isEmpty())
        return;
    switchToItem(ui->comboBox_playlists, name);
}

void PreferencesDialog::switchFile(const QString &url)
{
    if (url.isEmpty())
        return;
    switchToItem(ui->comboBox_url, url);
}

void PreferencesDialog::mediaEndReached()
{
    const SettingsManager::PlaybackMode mode = SettingsManager::getInstance()->getPlaybackMode();
    switch (mode)
    {
    case SettingsManager::PlaybackMode::RepeatCurrentPlaylist:
        if (ui->comboBox_url->count() > 1)
            playNextMedia();
        ui->pushButton_play->click();
        break;
    case SettingsManager::PlaybackMode::RepeatAllPlaylists:
        if (ui->comboBox_playlists->count() > 1)
        {
            if (ui->comboBox_url->currentIndex() == (ui->comboBox_url->count() - 1))
                playNextPlaylist();
            else
                playNextMedia();
        }
        ui->pushButton_play->click();
        break;
    case SettingsManager::PlaybackMode::RandomFileFromCurrentPlaylist:
        switchToRandomFile();
        ui->pushButton_play->click();
        break;
    case SettingsManager::PlaybackMode::RandomFileFromAllPlaylists:
        playRandomFileFromAllPlaylistsFiles();
        ui->pushButton_play->click();
        break;
    case SettingsManager::PlaybackMode::RandomPlaylist:
        switchToRandomPlaylist();
        ui->pushButton_play->click();
        break;
    default:
        ui->pushButton_play->click();
        break;
    }
}

void PreferencesDialog::playRandomFileFromAllPlaylistsFiles()
{
    int totalFileCount = 0;
    int playlistFileCount[100] = { 0 };
    QStringList playlists = SettingsManager::getInstance()->getAllPlaylistNames();
    if (!playlists.isEmpty())
    {
        for (int i = 0; i != playlists.count(); ++i)
        {
            QStringList paths = SettingsManager::getInstance()->getAllFilesFromPlaylist(playlists.at(i));
            if (!paths.isEmpty())
            {
                playlistFileCount[i] = paths.count();
                totalFileCount += playlistFileCount[i];
            }
        }
        if (totalFileCount > 1)
        {
            int index = getRandomNumber(totalFileCount - 1);
            index = index < 0 ? 0 : index;
            if (index < playlistFileCount[0])
            {
                if (ui->comboBox_playlists->currentIndex() != 0)
                    ui->comboBox_playlists->setCurrentIndex(0);
                if (ui->comboBox_url->currentIndex() != index)
                    ui->comboBox_url->setCurrentIndex(index);
            }
            else
                for (int i = 1; i != playlists.count(); ++i)
                {
                    index -= playlistFileCount[i - 1];
                    if (index < playlistFileCount[i])
                    {
                        if (ui->comboBox_playlists->currentIndex() != i)
                            ui->comboBox_playlists->setCurrentIndex(i);
                        if (ui->comboBox_url->currentIndex() != index)
                            ui->comboBox_url->setCurrentIndex(index);
                        break;
                    }
                }
        }
    }
}

void PreferencesDialog::switchToRandomFile()
{
    switchRandomItem(ui->comboBox_url);
}

void PreferencesDialog::switchToRandomPlaylist()
{
    switchRandomItem(ui->comboBox_playlists);
}

int PreferencesDialog::getRandomNumber(int max)
{
    int Max = max + 1;
    if (Max < 1)
        return -1;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int num = qrand() % Max;
    return num;
}

void PreferencesDialog::clearAllTracks()
{
    ui->comboBox_video_track->clear();
    ui->comboBox_audio_track->clear();
    ui->comboBox_subtitle_track->clear();
}

PreferencesDialog::PreferencesDialog(QWidget *parent) : CFramelessWindow(parent)
{
    ui = new Ui::PreferencesDialog();
    ui->setupUi(this);
    setContentsMargins(0, 0, 0, 0);
    setResizeable(true);
    setResizeableAreaWidth(5);
    setTitleBar(ui->widget_windowTitleBar);
    addIgnoreWidget(ui->label_windowTitle);
    initUI();
    initIcons();
    Utils::enableBlurBehindWindow(this);
    initConnections();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
#ifndef DD_NO_WIN_EXTRAS
    delete taskbarButton;
#endif
}

#if !defined(DD_NO_DRAG_DROP) && !defined(DD_NO_MIME_TYPE)
static bool canHandleDrop(const QDragEnterEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.empty()) return false;
    QMimeDatabase mimeDatabase;
    return SettingsManager::getInstance()->getSupportedMimeTypes().
            contains(mimeDatabase.mimeTypeForUrl(urls.constFirst()).name());
}

void PreferencesDialog::dragEnterEvent(QDragEnterEvent *event)
{
    CFramelessWindow::dragEnterEvent(event);
    event->setAccepted(canHandleDrop(event));
}

void PreferencesDialog::dropEvent(QDropEvent *event)
{
    CFramelessWindow::dropEvent(event);
    event->accept();
    QUrl url = event->mimeData()->urls().constFirst();
    QString path;
    if (url.isLocalFile())
        path = QDir::toNativeSeparators(QDir::cleanPath(url.toLocalFile()));
    else
        path = url.url();
    if (ui->comboBox_url->findText(path) < 0)
        ui->comboBox_url->insertItem(ui->comboBox_url->currentIndex() + 1, path);
    if (ui->comboBox_url->currentText() != path)
    {
        ui->comboBox_url->setCurrentText(path);
        QStringList files;
        for (int i = 0; i != ui->comboBox_url->count(); ++i)
            files.append(ui->comboBox_url->itemText(i));
        SettingsManager::getInstance()->setPlaylistFiles(SettingsManager::getInstance()->getCurrentPlaylistName(), files);
    }
}
#endif

void PreferencesDialog::closeEvent(QCloseEvent *event)
{
    CFramelessWindow::closeEvent(event);
    if (ui->comboBox_url->currentText().isEmpty())
        QApplication::quit();
}

void PreferencesDialog::initUI()
{
#ifndef DD_NO_WIN_EXTRAS
    taskbarButton = new QWinTaskbarButton();
    taskbarButton->setWindow(windowHandle());
    taskbarProgress = taskbarButton->progress();
    taskbarProgress->setRange(0, 99);
    taskbarProgress->show();
#endif
    ui->comboBox_opengl_type->addItem(QStringLiteral("OpenGLES (recommended)"), QStringLiteral("egl"));
    ui->comboBox_opengl_type->addItem(QStringLiteral("Desktop"), QStringLiteral("gl"));
    ui->comboBox_opengl_type->addItem(QStringLiteral("Software (very slow!)"), QStringLiteral("sw"));
    ui->comboBox_opengl_type->addItem(QStringLiteral("Auto"), QStringLiteral("auto"));
    int i = ui->comboBox_opengl_type->findData(SettingsManager::getInstance()->getOpenGLType());
    ui->comboBox_opengl_type->setCurrentIndex(i > -1 ? i : 0);
    populatePlaylists();
    populateFiles();
    ui->comboBox_playback_mode->setCurrentIndex(SettingsManager::getInstance()->getPlaybackMode());
    ui->checkBox_auto_update->setChecked(SettingsManager::getInstance()->getAutoCheckUpdate());
    ui->comboBox_video_track->setEnabled(false);
    ui->comboBox_audio_track->setEnabled(false);
    ui->comboBox_subtitle_track->setEnabled(false);
    ui->comboBox_image_quality->addItem(DD_TR("Best"), QStringLiteral("best"));
    ui->comboBox_image_quality->addItem(DD_TR("Fastest"), QStringLiteral("fastest"));
    ui->comboBox_image_quality->addItem(DD_TR("Default"), QStringLiteral("default"));
    ui->comboBox_video_renderer->addItem(QStringLiteral("OpenGLWidget"), Utils::getVideoRendererId(Utils::VideoRendererId::OpenGLWidget));
    ui->comboBox_video_renderer->addItem(QStringLiteral("QGLWidget2 (recommended)"), Utils::getVideoRendererId(Utils::VideoRendererId::GLWidget2));
    ui->comboBox_video_renderer->addItem(QStringLiteral("Widget"), Utils::getVideoRendererId(Utils::VideoRendererId::Widget));
    ui->comboBox_video_renderer->addItem(QStringLiteral("GDI"), Utils::getVideoRendererId(Utils::VideoRendererId::GDI));
    ui->comboBox_video_renderer->addItem(QStringLiteral("Direct2D"), Utils::getVideoRendererId(Utils::VideoRendererId::Direct2D));
#ifndef DD_NO_CSS
    ui->comboBox_skin->addItem(DD_TR("<None>"), QStringLiteral("none"));
    populateSkins(QStringLiteral(":/skins"));
    const QString skinDirPath = QApplication::applicationDirPath() + QStringLiteral("/skins");
    populateSkins(skinDirPath, true, true);
#else
    ui->comboBox_skin->setEnabled(false);
#endif
#ifndef DD_NO_TRANSLATIONS
    ui->comboBox_language->addItem(DD_TR("Auto"), QStringLiteral("auto"));
    ui->comboBox_language->addItem(QStringLiteral("American English"), QStringLiteral("en"));
    populateLanguages(QStringLiteral(":/i18n"));
#ifdef BUILD_DD_STATIC
    const QString i18nDirPath = QApplication::applicationDirPath() + QStringLiteral("/translations");
#else
    const QString i18nDirPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
    populateLanguages(i18nDirPath, true, true);
#else
    ui->comboBox_language->setEnabled(false);
#endif
    ui->comboBox_subtitle_charset->addItem(DD_TR("Auto detect"), QStringLiteral("AutoDetect"));
    ui->comboBox_subtitle_charset->addItem(DD_TR("System"), QStringLiteral("System"));
    for (auto& codec : QTextCodec::availableCodecs())
        ui->comboBox_subtitle_charset->addItem(QString::fromLatin1(codec), QString::fromLatin1(codec));
    if (audioAvailable)
    {
        ui->checkBox_volume->setChecked(!SettingsManager::getInstance()->getMute());
        ui->horizontalSlider_volume->setEnabled(ui->checkBox_volume->isChecked());
        ui->horizontalSlider_volume->setValue(SettingsManager::getInstance()->getVolume());
    }
    ui->checkBox_autoStart->setChecked(Win32Utils::isAutoStartServiceInstalled(TEXT("ddassvc")));
    QStringList decoders = SettingsManager::getInstance()->getDecoders();
    ui->checkBox_hwdec_cuda->setChecked(decoders.contains(QStringLiteral("CUDA")));
    ui->checkBox_hwdec_d3d11->setChecked(decoders.contains(QStringLiteral("D3D11")));
    ui->checkBox_hwdec_dxva->setChecked(decoders.contains(QStringLiteral("DXVA")));
    bool hwdecEnabled = (ui->checkBox_hwdec_cuda->isChecked()
                         || ui->checkBox_hwdec_d3d11->isChecked()
                         || ui->checkBox_hwdec_dxva->isChecked())
            && SettingsManager::getInstance()->getHwdec();
    ui->checkBox_hwdec->setChecked(hwdecEnabled);
    ui->checkBox_hwdec_cuda->setEnabled(hwdecEnabled);
    ui->checkBox_hwdec_d3d11->setEnabled(hwdecEnabled);
    ui->checkBox_hwdec_dxva->setEnabled(hwdecEnabled);
    ui->radioButton_ratio_fitDesktop->setChecked(SettingsManager::getInstance()->getFitDesktop());
    ui->radioButton_ratio_videoAspectRatio->setChecked(!ui->radioButton_ratio_fitDesktop->isChecked());
    i = ui->comboBox_subtitle_charset->findData(SettingsManager::getInstance()->getCharset());
    ui->comboBox_subtitle_charset->setCurrentIndex(i > -1 ? i : 0);
    ui->checkBox_subtitle_autoLoadExternal->setChecked(SettingsManager::getInstance()->getSubtitleAutoLoad());
    ui->checkBox_displaySubtitle->setChecked(SettingsManager::getInstance()->getSubtitle());
    ui->checkBox_audio_autoLoadExternal->setChecked(SettingsManager::getInstance()->getAudioAutoLoad());
#ifndef DD_NO_CSS
    i = ui->comboBox_skin->findData(SettingsManager::getInstance()->getSkin());
    ui->comboBox_skin->setCurrentIndex(i > -1 ? i : 0);
#endif
#ifndef DD_NO_TRANSLATIONS
    i = ui->comboBox_language->findData(SettingsManager::getInstance()->getLanguage());
    ui->comboBox_language->setCurrentIndex(i > -1 ? i : 0);
#endif
    const int vid = SettingsManager::getInstance()->getRenderer();
    i = ui->comboBox_video_renderer->findData(vid <= 0 ? Utils::getVideoRendererId(Utils::VideoRendererId::GLWidget2) : vid);
    ui->comboBox_video_renderer->setCurrentIndex(i > -1 ? i : 0);
    i = ui->comboBox_image_quality->findData(SettingsManager::getInstance()->getImageQuality());
    ui->comboBox_image_quality->setCurrentIndex(i > -1 ? i : 0);
}

void PreferencesDialog::initConnections()
{
    connect(ui->pushButton_preferencesDialog_previous, &QPushButton::clicked, this, &PreferencesDialog::playPreviousMedia);
    connect(ui->pushButton_preferencesDialog_next, &QPushButton::clicked, this, &PreferencesDialog::playNextMedia);
    connect(ui->pushButton_edit_playlist, &QPushButton::clicked, this, &PreferencesDialog::showPlaylistDialog);
    connect(ui->checkBox_auto_update, &QCheckBox::clicked, this, [=]
    {
        bool autoUpdate = ui->checkBox_auto_update->isChecked();
        if (autoUpdate != SettingsManager::getInstance()->getAutoCheckUpdate())
            SettingsManager::getInstance()->setAutoCheckUpdate(autoUpdate);
    });
#ifndef DD_NO_WIN_EXTRAS
    connect(ui->horizontalSlider_video_position, &QSlider::valueChanged, taskbarProgress, &QWinTaskbarProgress::setValue);
    connect(ui->horizontalSlider_video_position, &QSlider::rangeChanged, taskbarProgress, &QWinTaskbarProgress::setRange);
#endif
    connect(ui->pushButton_audio_open, &QPushButton::clicked, this, [=]
    {
        QString audioPath = QFileDialog::getOpenFileName(nullptr, DD_TR("Please select an audio file"), SettingsManager::getInstance()->getLastDir(), DD_TR("Audios (*.mka *.aac *.flac *.mp3 *.wav);;All files (*)"));
        if (!audioPath.isEmpty())
            emit this->audioFileChanged(audioPath);
    });
    connect(ui->pushButton_subtitle_open, &QPushButton::clicked, this, [=]
    {
        QString subtitlePath = QFileDialog::getOpenFileName(nullptr, DD_TR("Please select a subtitle file"), SettingsManager::getInstance()->getLastDir(), DD_TR("Subtitles (*.ass *.ssa *.srt *.sub);;All files (*)"));
        if (!subtitlePath.isEmpty())
            emit this->subtitleFileChanged(subtitlePath);
    });
    connect(ui->horizontalSlider_volume, &QSlider::sliderMoved, this, [=](int value)
    {
        int vol = value;
        if (vol > 99)
            vol = 99;
        if (vol < 0)
            vol = 0;
        if (static_cast<quint32>(vol) != SettingsManager::getInstance()->getVolume())
        {
            SettingsManager::getInstance()->setVolume(static_cast<quint32>(vol));
            emit this->volumeChanged(SettingsManager::getInstance()->getVolume());
        }
    });
    connect(ui->horizontalSlider_video_position, &QSlider::sliderMoved, this, [=](int value)
    {
        emit this->seek(static_cast<qint64>(value * sliderUnit));
    });
    connect(ui->pushButton_about, &QPushButton::clicked, this, &PreferencesDialog::about);
    connect(ui->pushButton_minimize, &QPushButton::clicked, this, &PreferencesDialog::showMinimized);
    connect(ui->pushButton_close, &QPushButton::clicked, this, &PreferencesDialog::close);
    connect(ui->checkBox_volume, &QCheckBox::clicked, this, [=]
    {
        ui->horizontalSlider_volume->setEnabled(ui->checkBox_volume->isChecked());
        if (ui->checkBox_volume->isChecked() != !SettingsManager::getInstance()->getMute())
        {
            SettingsManager::getInstance()->setMute(!ui->checkBox_volume->isChecked());
            emit this->muteChanged(SettingsManager::getInstance()->getMute());
        }
    });
    connect(ui->pushButton_play, &QPushButton::clicked, this, [=]
    {
        if (!ui->comboBox_url->currentText().isEmpty() && (ui->comboBox_url->currentText() != SettingsManager::getInstance()->getLastFile()))
        {
            SettingsManager::getInstance()->setLastFile(ui->comboBox_url->currentText());
            emit this->urlChanged(SettingsManager::getInstance()->getLastFile());
        }
        else
            emit this->play();
#ifndef DD_NO_WIN_EXTRAS
        if (!taskbarProgress->isVisible())
            taskbarProgress->show();
        taskbarProgress->resume();
#endif
    });
    connect(ui->pushButton_pause, &QPushButton::clicked, this, [=]
    {
#ifndef DD_NO_WIN_EXTRAS
        taskbarProgress->pause();
#endif
        emit this->pause();
    });
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &PreferencesDialog::close);
    connect(ui->comboBox_opengl_type, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        Q_UNUSED(index)
        if (ui->comboBox_opengl_type->currentData().toString() != SettingsManager::getInstance()->getOpenGLType())
        {
            SettingsManager::getInstance()->setOpenGLType(ui->comboBox_opengl_type->currentData().toString());
            QMessageBox::information(nullptr, DD_TR("Information"), DD_TR("Due to some limitations of Qt, this option will not take effect until you restart this application."));
        }
    });
    connect(ui->checkBox_hwdec, &QCheckBox::clicked, this, [=]
    {
        bool hwdecEnabled = ui->checkBox_hwdec->isChecked();
        ui->checkBox_hwdec_cuda->setEnabled(hwdecEnabled);
        ui->checkBox_hwdec_d3d11->setEnabled(hwdecEnabled);
        ui->checkBox_hwdec_dxva->setEnabled(hwdecEnabled);
        this->setDecoders();
    });
    connect(ui->checkBox_hwdec_cuda, &QCheckBox::clicked, this, &PreferencesDialog::setDecoders);
    connect(ui->checkBox_hwdec_d3d11, &QCheckBox::clicked, this, &PreferencesDialog::setDecoders);
    connect(ui->checkBox_hwdec_dxva, &QCheckBox::clicked, this, &PreferencesDialog::setDecoders);
#ifndef DD_NO_CSS
    connect(ui->comboBox_skin, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        Q_UNUSED(index)
        if (ui->comboBox_skin->currentData().toString() != SettingsManager::getInstance()->getSkin())
        {
            SettingsManager::getInstance()->setSkin(ui->comboBox_skin->currentData().toString());
            SkinsManager::getInstance()->setSkin(SettingsManager::getInstance()->getSkin());
            emit this->skinChanged(SettingsManager::getInstance()->getSkin());
        }
    });
#endif
#ifndef DD_NO_TRANSLATIONS
    connect(ui->comboBox_language, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        Q_UNUSED(index)
        if (ui->comboBox_language->currentData().toString() != SettingsManager::getInstance()->getLanguage())
        {
            SettingsManager::getInstance()->setLanguage(ui->comboBox_language->currentData().toString());
            emit this->languageChanged(SettingsManager::getInstance()->getLanguage());
        }
    });
#endif
    connect(ui->comboBox_playlists, &QComboBox::currentTextChanged, this, [=](const QString &text)
    {
        if (!refreshingData && !text.isEmpty() && (text != SettingsManager::getInstance()->getCurrentPlaylistName()))
        {
            SettingsManager::getInstance()->setCurrentPlaylistName(text);
            populateFiles();
            emit this->playlistChanged(text);
        }
    });
    connect(ui->comboBox_playback_mode, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        auto mode = static_cast<SettingsManager::PlaybackMode>(index);
        if (mode != SettingsManager::getInstance()->getPlaybackMode())
        {
            SettingsManager::getInstance()->setPlaybackMode(mode);
            emit this->playbackModeChanged(static_cast<quint32>(index));
            emit this->repeatCurrentFile(mode == SettingsManager::PlaybackMode::RepeatCurrentFile);
        }
    });
    connect(ui->comboBox_video_track, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        Q_UNUSED(index)
        emit this->videoTrackChanged(ui->comboBox_video_track->currentData().toUInt());
    });
    connect(ui->comboBox_audio_track, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        Q_UNUSED(index)
        emit this->audioTrackChanged(ui->comboBox_audio_track->currentData().toUInt());
    });
    connect(ui->comboBox_subtitle_track, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        Q_UNUSED(index)
        emit this->subtitleTrackChanged(ui->comboBox_subtitle_track->currentData());
    });
    connect(ui->comboBox_video_renderer, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        Q_UNUSED(index)
        if (ui->comboBox_video_renderer->currentData().toInt() != SettingsManager::getInstance()->getRenderer())
        {
            SettingsManager::getInstance()->setRenderer(ui->comboBox_video_renderer->currentData().toInt());
            emit this->rendererChanged(SettingsManager::getInstance()->getRenderer());
        }
    });
    connect(ui->comboBox_image_quality, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        Q_UNUSED(index)
        if (ui->comboBox_image_quality->currentData().toString() != SettingsManager::getInstance()->getImageQuality())
        {
            SettingsManager::getInstance()->setImageQuality(ui->comboBox_image_quality->currentData().toString());
            emit this->imageQualityChanged(SettingsManager::getInstance()->getImageQuality());
        }
    });
    connect(ui->comboBox_url, &QComboBox::currentTextChanged, this, [=](const QString &text)
    {
        if (!refreshingData && !text.isEmpty() && (text != SettingsManager::getInstance()->getLastFile()))
        {
            SettingsManager::getInstance()->setLastFile(text);
            emit this->urlChanged(SettingsManager::getInstance()->getLastFile());
        }
    });
    connect(ui->checkBox_autoStart, &QCheckBox::clicked, this, [=]
    {
        QString servicePath = QApplication::applicationDirPath() + QStringLiteral("/DDService");
#ifdef _DEBUG
        servicePath += QStringLiteral("d");
#endif
        servicePath += QStringLiteral(".exe");
        servicePath = QDir::toNativeSeparators(QDir::cleanPath(servicePath));
        if (!QFileInfo::exists(servicePath))
        {
            ui->checkBox_autoStart->setChecked(false);
            ui->checkBox_autoStart->setEnabled(false);
            return;
        }
        if (ui->checkBox_autoStart->isChecked() && !Win32Utils::isAutoStartServiceInstalled(TEXT("ddassvc")))
            Utils::run(servicePath, QStringList() << QStringLiteral("-i"), true, true);
        else if (!ui->checkBox_autoStart->isChecked() && Win32Utils::isAutoStartServiceInstalled(TEXT("ddassvc")))
            Utils::run(servicePath, QStringList() << QStringLiteral("-u"), true, true);
        QTimer::singleShot(2500, this, [=]
        {
            bool isAutoStart = Win32Utils::isAutoStartServiceInstalled(TEXT("ddassvc"));
            if (ui->checkBox_autoStart->isChecked() != isAutoStart)
                ui->checkBox_autoStart->setChecked(isAutoStart);
        });
    });
    connect(ui->radioButton_ratio_fitDesktop, &QRadioButton::clicked, this, &PreferencesDialog::setRatio);
    connect(ui->radioButton_ratio_videoAspectRatio, &QRadioButton::clicked, this, &PreferencesDialog::setRatio);
    connect(ui->comboBox_subtitle_charset, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index)
    {
        Q_UNUSED(index)
        if (ui->comboBox_subtitle_charset->currentData().toString() != SettingsManager::getInstance()->getCharset())
        {
            SettingsManager::getInstance()->setCharset(ui->comboBox_subtitle_charset->currentData().toString());
            emit this->charsetChanged(SettingsManager::getInstance()->getCharset());
        }
    });
    connect(ui->checkBox_subtitle_autoLoadExternal, &QCheckBox::clicked, this, [=]
    {
        if (ui->checkBox_subtitle_autoLoadExternal->isChecked() != SettingsManager::getInstance()->getSubtitleAutoLoad())
        {
            SettingsManager::getInstance()->setSubtitleAutoLoad(ui->checkBox_subtitle_autoLoadExternal->isChecked());
            emit this->subtitleAutoLoadChanged(SettingsManager::getInstance()->getSubtitleAutoLoad());
        }
    });
    connect(ui->checkBox_displaySubtitle, &QCheckBox::clicked, this, [=]
    {
        if (ui->checkBox_displaySubtitle->isChecked() != SettingsManager::getInstance()->getSubtitle())
        {
            SettingsManager::getInstance()->setSubtitle(ui->checkBox_displaySubtitle->isChecked());
            emit this->subtitleEnableChanged(SettingsManager::getInstance()->getSubtitle());
        }
    });
}

void PreferencesDialog::setDecoders()
{
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
    if (isVisible() && isActiveWindow())
        QMessageBox::information(nullptr, QStringLiteral("Dynamic Desktop"), DD_TR("Reopen this video or play another video to experience it.\nMake sure this application runs in your GPU's Optimus mode."));
}

void PreferencesDialog::setRatio()
{
    if (ui->radioButton_ratio_fitDesktop->isChecked() != SettingsManager::getInstance()->getFitDesktop())
    {
        SettingsManager::getInstance()->setFitDesktop(ui->radioButton_ratio_fitDesktop->isChecked());
        emit this->imageRatioChanged(SettingsManager::getInstance()->getFitDesktop());
    }
}

#ifndef DD_NO_TRANSLATIONS
void PreferencesDialog::refreshTexts(const QString &language)
{
    Q_UNUSED(language)
    ui->retranslateUi(this);
}
#endif

void PreferencesDialog::setMute(bool mute)
{
    ui->checkBox_volume->setChecked(!mute);
    emit ui->checkBox_volume->clicked(!mute);
}

void PreferencesDialog::setPlaying(bool playing)
{
    if (isPlaying != playing)
        isPlaying = playing;
}

void PreferencesDialog::togglePlayPause()
{
    if (isPlaying)
        emit ui->pushButton_pause->clicked();
    else
        emit ui->pushButton_play->clicked();
}

void PreferencesDialog::initIcons()
{
#ifndef DD_NO_SVG
    ui->label_windowIcon->setPixmap(QPixmap(QStringLiteral(":/icons/color_palette.svg")));
    ui->pushButton_minimize->setIcon(QIcon(QStringLiteral(":/icons/minimize.svg")));
    ui->pushButton_close->setIcon(QIcon(QStringLiteral(":/icons/close.svg")));
#else
    ui->label_windowIcon->setPixmap(QPixmap(QStringLiteral(":/icons/color_palette.png")));
    ui->pushButton_minimize->setIcon(QIcon(QStringLiteral(":/icons/minimize.png")));
    ui->pushButton_close->setIcon(QIcon(QStringLiteral(":/icons/close.png")));
#endif
}

void PreferencesDialog::populateFiles()
{
    if (ui->comboBox_url->count() > 0)
        ui->comboBox_url->clear();
    ui->comboBox_url->addItems(SettingsManager::getInstance()->getAllFilesFromPlaylist(SettingsManager::getInstance()->getCurrentPlaylistName()));
    int i = ui->comboBox_url->findText(SettingsManager::getInstance()->getLastFile());
    ui->comboBox_url->setCurrentIndex(i >= 0 ? i : 0);
}

void PreferencesDialog::populatePlaylists()
{
    if (ui->comboBox_playlists->count() > 0)
        ui->comboBox_playlists->clear();
    ui->comboBox_playlists->addItems(SettingsManager::getInstance()->getAllPlaylistNames());
    int i = ui->comboBox_playlists->findText(SettingsManager::getInstance()->getCurrentPlaylistName());
    ui->comboBox_playlists->setCurrentIndex(i >= 0 ? i : 0);
}

void PreferencesDialog::moveNextItem(QComboBox *comboBox)
{
    if (comboBox == nullptr)
        return;
    if (comboBox->count() <= 1)
        return;
    if (comboBox->currentIndex() < (comboBox->count() - 1))
        comboBox->setCurrentIndex(comboBox->currentIndex() + 1);
    else
        comboBox->setCurrentIndex(0);
}

void PreferencesDialog::movePreviousItem(QComboBox *comboBox)
{
    if (comboBox == nullptr)
        return;
    if (comboBox->count() <= 1)
        return;
    if (comboBox->currentIndex() > 0)
        comboBox->setCurrentIndex(comboBox->currentIndex() - 1);
    else
        comboBox->setCurrentIndex(comboBox->count() - 1);
}

void PreferencesDialog::switchRandomItem(QComboBox *comboBox)
{
    if (comboBox == nullptr)
        return;
    if (comboBox->count() < 2)
        return;
    int index = getRandomNumber(comboBox->count() - 1);
    index = index < 0 ? 0 : index;
    comboBox->setCurrentIndex(index);
}

void PreferencesDialog::switchToItem(QComboBox *comboBox, const QString &text)
{
    if (comboBox == nullptr)
        return;
    if (text.isEmpty())
        return;
    int index = comboBox->findText(text);
    if (index < 0)
        return;
    comboBox->setCurrentIndex(index);
}
