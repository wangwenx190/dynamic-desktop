#include "playlistdialog.h"
#include "ui_playlistdialog.h"

#include "settingsmanager.h"

#include <QInputDialog>
#include <QFileDialog>

PlaylistDialog::PlaylistDialog(QWidget *parent) : QWidget(parent)
{
    ui = new Ui::PlaylistDialog();
    ui->setupUi(this);
    populatePlaylists();
    populateFiles(SettingsManager::getInstance()->getCurrentPlaylistName());
    connect(ui->listWidget_playlist, &QListWidget::currentTextChanged, this, &PlaylistDialog::populateFiles);
    connect(ui->pushButton_file_add, &QPushButton::clicked, this, [=]
    {
        QStringList paths = QFileDialog::getOpenFileNames(nullptr, DD_TR("Please select a media file"), SettingsManager::getInstance()->getLastDir(), DD_TR("Videos (*.avi *.mp4 *.mkv *.flv);;Audios (*.mp3 *.flac *.ape *.wav);;Pictures (*.bmp *.jpg *.jpeg *.png *.gif);;All files (*)"));
        bool changed = false;
        if (!paths.isEmpty())
            for (auto& path : paths)
            {
                const QString path2 = QDir::toNativeSeparators(QDir::cleanPath(path));
                if (ui->listWidget_file->findItems(path2, Qt::MatchFixedString).isEmpty())
                {
                    ui->listWidget_file->addItem(path2);
                    if (!changed)
                        changed = true;
                }
            }
        if (changed)
        {
            SettingsManager::getInstance()->setPlaylistFiles(SettingsManager::getInstance()->getCurrentPlaylistName(), paths);
        }
    });
}

PlaylistDialog::~PlaylistDialog()
{
    delete ui;
}

void PlaylistDialog::populatePlaylists()
{
    if (ui->listWidget_playlist->count() > 0)
        ui->listWidget_playlist->clear();
    ui->listWidget_playlist->addItems(SettingsManager::getInstance()->getAllPlaylistNames());
    //ui->listWidget_playlist->setCurrentItem(new QListWidgetItem(SettingsManager::getInstance()->getCurrentPlaylistName()));
    ui->listWidget_playlist->scrollToItem(ui->listWidget_playlist->currentItem());
}

void PlaylistDialog::populateFiles(const QString &name)
{
    if (name == currentPlaylist)
        return;
    if (ui->listWidget_file->count() > 0)
        ui->listWidget_file->clear();
    ui->listWidget_file->addItems(SettingsManager::getInstance()->getAllFilesFromPlaylist(name));
    //ui->listWidget_file->setCurrentItem(new QListWidgetItem(SettingsManager::getInstance()->getLastFile()));
    ui->listWidget_file->scrollToItem(ui->listWidget_file->currentItem());
    currentPlaylist = name;
    SettingsManager::getInstance()->setCurrentPlaylistName(currentPlaylist);
}
