#include "playlistdialog.h"
#include "ui_playlistdialog.h"

#include "settingsmanager.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>

PlaylistDialog::PlaylistDialog(QWidget *parent) : QWidget(parent)
{
    ui = new Ui::PlaylistDialog();
    ui->setupUi(this);
    populatePlaylists();
    populateFiles(SettingsManager::getInstance()->getCurrentPlaylistName());
    connect(ui->pushButton_playlist_add, &QPushButton::clicked, this, [=]
    {
        bool ok = false;
        QString input = QInputDialog::getText(nullptr, DD_TR("Please input a valid playlist name"), DD_TR("Playlist name"), QLineEdit::Normal, QStringLiteral("Default"), &ok);
        if (ok && !input.isEmpty())
        {
            bool changed = false;
            if (findItem(ui->listWidget_playlist, input) < 0)
            {
                ui->listWidget_playlist->addItem(input);
                if (!changed)
                    changed = true;
            }
            setCurrentItem(ui->listWidget_playlist, input);
            if (changed)
                SettingsManager::getInstance()->setAllPlaylistNames(getAllItems(ui->listWidget_playlist));
        }
    });
    connect(ui->pushButton_playlist_remove, &QPushButton::clicked, this, [=]
    {
        if (ui->listWidget_playlist->count() < 2)
            QMessageBox::warning(nullptr, QStringLiteral("Dynamic Desktop"), DD_TR("There has to be at least one playlist."));
        else
        {
            int index = ui->listWidget_playlist->currentRow();
            ui->listWidget_playlist->takeItem(index);
            /*if (index < ui->listWidget_playlist->count())
                ui->listWidget_playlist->setCurrentRow(index);
            else
                ui->listWidget_playlist->setCurrentRow(index - 1);
            ui->listWidget_playlist->scrollToItem(ui->listWidget_playlist->currentItem());*/
            SettingsManager::getInstance()->setAllPlaylistNames(getAllItems(ui->listWidget_playlist));
        }
    });
    connect(ui->pushButton_playlist_rename, &QPushButton::clicked, this, [=]{});
    connect(ui->pushButton_playlist_up, &QPushButton::clicked, this, [=]{});
    connect(ui->pushButton_playlist_down, &QPushButton::clicked, this, [=]{});
    connect(ui->listWidget_playlist, &QListWidget::currentTextChanged, this, [=](const QString &text)
    {
        if (text != SettingsManager::getInstance()->getCurrentPlaylistName())
        {
            SettingsManager::getInstance()->setCurrentPlaylistName(text);
            populateFiles(SettingsManager::getInstance()->getCurrentPlaylistName());
        }
    });
    connect(ui->pushButton_file_add, &QPushButton::clicked, this, [=]
    {
        QStringList paths = QFileDialog::getOpenFileNames(nullptr, DD_TR("Please select a media file"), SettingsManager::getInstance()->getLastDir(), DD_TR("Videos (*.avi *.mp4 *.mkv *.flv);;Audios (*.mp3 *.flac *.ape *.wav);;Pictures (*.bmp *.jpg *.jpeg *.png *.gif);;All files (*)"));
        bool changed = false;
        if (!paths.isEmpty())
            if (paths.count() == 1)
            {
                const QString path2 = QDir::toNativeSeparators(QDir::cleanPath(paths.constFirst()));
                if (findItem(ui->listWidget_file, path2) < 0)
                    ui->listWidget_file->addItem(path2);
                setCurrentItem(ui->listWidget_file, path2);
            }
            else
                for (auto& path : paths)
                {
                    const QString path2 = QDir::toNativeSeparators(QDir::cleanPath(path));
                    if (findItem(ui->listWidget_file, path2) < 0)
                    {
                        ui->listWidget_file->addItem(path2);
                        if (!changed)
                            changed = true;
                    }
                }
        if (changed)
            SettingsManager::getInstance()->setPlaylistFiles(SettingsManager::getInstance()->getCurrentPlaylistName(), getAllItems(ui->listWidget_file));
    });
    connect(ui->pushButton_file_input, &QPushButton::clicked, this, [=]{});
    connect(ui->pushButton_file_remove, &QPushButton::clicked, this, [=]
    {
        if (ui->listWidget_file->count() < 2)
        {
            if (QMessageBox::question(nullptr, QStringLiteral("Dynamic Desktop"), DD_TR("It seems that there is only one file left in the current playlist.\nDo you want to remove the whole playlist?")) == QMessageBox::Yes)
                ui->pushButton_playlist_remove->click();
        }
        else
        {
            int index = ui->listWidget_file->currentRow();
            ui->listWidget_file->takeItem(index);
            /*if (index < ui->listWidget_file->count())
                ui->listWidget_file->setCurrentRow(index);
            else
                ui->listWidget_file->setCurrentRow(index - 1);
            ui->listWidget_file->scrollToItem(ui->listWidget_file->currentItem());*/
            SettingsManager::getInstance()->setPlaylistFiles(SettingsManager::getInstance()->getCurrentPlaylistName(), getAllItems(ui->listWidget_file));
        }
    });
    connect(ui->pushButton_file_up, &QPushButton::clicked, this, [=]{});
    connect(ui->pushButton_file_down, &QPushButton::clicked, this, [=]{});
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
    setCurrentItem(ui->listWidget_playlist, SettingsManager::getInstance()->getCurrentPlaylistName());
}

void PlaylistDialog::populateFiles(const QString &name)
{
    if (ui->listWidget_file->count() > 0)
        ui->listWidget_file->clear();
    ui->listWidget_file->addItems(SettingsManager::getInstance()->getAllFilesFromPlaylist(name));
    setCurrentItem(ui->listWidget_file, SettingsManager::getInstance()->getLastFile());
}

QStringList PlaylistDialog::getAllItems(QListWidget *listWidget)
{
    if (listWidget == nullptr)
        return {};
    if (listWidget->count() < 1)
        return {};
    QStringList items;
    for (int i = 0; i != listWidget->count(); ++i)
        if (!listWidget->item(i)->text().isEmpty())
            items.append(listWidget->item(i)->text());
    return items;
}

int PlaylistDialog::findItem(QListWidget *listWidget, const QString &text)
{
    if ((listWidget == nullptr) || text.isEmpty())
        return -1;
    if (listWidget->count() < 1)
        return -1;
    for (int i = 0; i != listWidget->count(); ++i)
        if (text == listWidget->item(i)->text())
            return i;
    return -1;
}

void PlaylistDialog::setCurrentItem(QListWidget *listWidget, const QString &text)
{
    if ((listWidget == nullptr) || text.isEmpty())
        return;
    int index = findItem(listWidget, text);
    index = index < 0 ? 0 : index;
    listWidget->setCurrentRow(index);
    listWidget->scrollToItem(listWidget->item(index));
}
