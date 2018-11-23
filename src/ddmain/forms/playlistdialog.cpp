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
    currentPlaylist = SettingsManager::getInstance()->getCurrentPlaylistName();
    populatePlaylists();
    populateFiles(currentPlaylist);
    connect(ui->pushButton_playlist_add, &QPushButton::clicked, this, [=]
    {
        bool ok = false;
        QString input = QInputDialog::getText(nullptr, DD_TR("Please input a valid playlist name"), DD_TR("Playlist name"), QLineEdit::Normal, QStringLiteral("Default"), &ok);
        if (ok && !input.isEmpty())
            addPlaylist(input);
    });
    connect(ui->pushButton_playlist_remove, &QPushButton::clicked, this, [=]
    {
        if (ui->listWidget_playlist->count() < 2)
            QMessageBox::warning(nullptr, QStringLiteral("Dynamic Desktop"), DD_TR("There has to be at least one playlist."));
        else
            removePlaylist(currentPlaylist);
    });
    connect(ui->pushButton_playlist_rename, &QPushButton::clicked, this, [=]
    {
        bool ok = false;
        QString input = QInputDialog::getText(nullptr, DD_TR("Please input a valid playlist name"), DD_TR("Playlist name"), QLineEdit::Normal, QStringLiteral("Default"), &ok);
        if (ok && !input.isEmpty())
            renamePlaylist(ui->listWidget_playlist->currentItem()->text(), input);
    });
    connect(ui->pushButton_playlist_up, &QPushButton::clicked, this, [=]
    {
        itemMoveUp(ui->listWidget_playlist);
        SettingsManager::getInstance()->setAllPlaylistNames(getAllItems(ui->listWidget_playlist));
    });
    connect(ui->pushButton_playlist_down, &QPushButton::clicked, this, [=]
    {
        itemMoveDown(ui->listWidget_playlist);
        SettingsManager::getInstance()->setAllPlaylistNames(getAllItems(ui->listWidget_playlist));
    });
    connect(ui->listWidget_playlist, &QListWidget::currentTextChanged, this, [=](const QString &text)
    {
        if (text != currentPlaylist)
        {
            currentPlaylist = text;
            populateFiles(currentPlaylist);
        }
    });
    connect(ui->listWidget_playlist, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item)
    {
        emit this->switchPlaylist(item->text());
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
                {
                    ui->listWidget_file->addItem(path2);
                    if (!changed)
                        changed = true;
                }
                setCurrentItem(ui->listWidget_file, path2);
            }
            else
            {
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
                setCurrentItem(ui->listWidget_file, paths.constLast());
            }
        if (changed)
        {
            SettingsManager::getInstance()->setPlaylistFiles(currentPlaylist, getAllItems(ui->listWidget_file));
            emit this->dataRefreshed();
        }
    });
    connect(ui->pushButton_file_input, &QPushButton::clicked, this, [=]
    {
        bool ok = false;
        QString input = QInputDialog::getText(nullptr, DD_TR("Please input a valid URL"), DD_TR("URL"), QLineEdit::Normal, QStringLiteral("https://"), &ok);
        if (ok && !input.isEmpty())
        {
            bool changed = false;
            if (findItem(ui->listWidget_file, input) < 0)
            {
                ui->listWidget_file->addItem(input);
                if (!changed)
                    changed = true;
            }
            setCurrentItem(ui->listWidget_file, input);
            if (changed)
            {
                SettingsManager::getInstance()->setPlaylistFiles(currentPlaylist, getAllItems(ui->listWidget_file));
                emit this->dataRefreshed();
            }
        }
    });
    connect(ui->pushButton_file_remove, &QPushButton::clicked, this, [=]
    {
        if (ui->listWidget_file->count() < 2)
        {
            if (QMessageBox::question(nullptr, QStringLiteral("Dynamic Desktop"), DD_TR("It seems that there is only one file left in the current playlist.\nDo you want to remove the whole playlist?")) == QMessageBox::Yes)
                ui->pushButton_playlist_remove->click();
        }
        else
        {
            ui->listWidget_file->takeItem(ui->listWidget_file->currentRow());
            SettingsManager::getInstance()->setPlaylistFiles(currentPlaylist, getAllItems(ui->listWidget_file));
            emit this->dataRefreshed();
        }
    });
    connect(ui->pushButton_file_up, &QPushButton::clicked, this, [=]
    {
        itemMoveUp(ui->listWidget_file);
        SettingsManager::getInstance()->setPlaylistFiles(currentPlaylist, getAllItems(ui->listWidget_file));
    });
    connect(ui->pushButton_file_down, &QPushButton::clicked, this, [=]
    {
        itemMoveDown(ui->listWidget_file);
        SettingsManager::getInstance()->setPlaylistFiles(currentPlaylist, getAllItems(ui->listWidget_file));
    });
    connect(ui->listWidget_file, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item)
    {
        if (currentPlaylist != SettingsManager::getInstance()->getCurrentPlaylistName())
            emit this->switchPlaylist(currentPlaylist);
        emit this->playFile(item->text());
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
    setCurrentItem(ui->listWidget_playlist, currentPlaylist);
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

void PlaylistDialog::moveItem(QListWidget *listWidget, int from, int to)
{
    if (listWidget == nullptr)
        return;
    if ((from == to) || (from < 0) || (to < 0))
        return;
    QListWidgetItem *item = listWidget->takeItem(from);
    listWidget->insertItem(to, item);
    setCurrentItem(listWidget, item->text());
}

void PlaylistDialog::itemMoveUp(QListWidget *listWidget)
{
    int index = listWidget->currentRow() - 1;
    index = index < 0 ? listWidget->count() - 1 : index;
    moveItem(listWidget, listWidget->currentRow(), index);
    emit dataRefreshed();
}

void PlaylistDialog::itemMoveDown(QListWidget *listWidget)
{
    int index = listWidget->currentRow() + 1;
    index = index >= listWidget->count() ? 0 : index;
    moveItem(listWidget, listWidget->currentRow(), index);
    emit dataRefreshed();
}

void PlaylistDialog::addPlaylist(const QString &name)
{
    if (name.isEmpty())
        return;
    bool changed = false;
    if (findItem(ui->listWidget_playlist, name) < 0)
    {
        ui->listWidget_playlist->addItem(name);
        if (!changed)
            changed = true;
    }
    setCurrentItem(ui->listWidget_playlist, name);
    if (changed)
    {
        SettingsManager::getInstance()->setAllPlaylistNames(getAllItems(ui->listWidget_playlist));
        emit dataRefreshed();
    }
}

void PlaylistDialog::removePlaylist(const QString &name)
{
    if (name.isEmpty())
        return;
    SettingsManager::getInstance()->clearPlaylist(name);
    ui->listWidget_playlist->takeItem(ui->listWidget_playlist->currentRow());
    SettingsManager::getInstance()->setAllPlaylistNames(getAllItems(ui->listWidget_playlist));
    emit dataRefreshed();
}

void PlaylistDialog::renamePlaylist(const QString &oldName, const QString &newName)
{
    if (oldName.isEmpty() || newName.isEmpty())
        return;
    if (newName == oldName)
    {
        setCurrentItem(ui->listWidget_playlist, newName);
        return;
    }
    QStringList oldFiles = SettingsManager::getInstance()->getAllFilesFromPlaylist(oldName);
    removePlaylist(oldName);
    SettingsManager::getInstance()->setPlaylistFiles(newName, oldFiles);
    addPlaylist(newName);
    emit dataRefreshed();
}
