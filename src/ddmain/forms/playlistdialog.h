#pragma once

#include <QtNiceFramelessWindow>

QT_FORWARD_DECLARE_CLASS(QListWidget)

namespace Ui
{
    class PlaylistDialog;
}

class PlaylistDialog : public CFramelessWindow
{
    Q_OBJECT

signals:
    void dataRefreshed();
    void switchPlaylist(const QString &);
    void playFile(const QString &);

public:
    explicit PlaylistDialog(QWidget *parent = nullptr);
    ~PlaylistDialog() override;

public slots:
#ifndef DD_NO_TRANSLATIONS
    void refreshTexts(const QString &language);
#endif

private slots:
    void populatePlaylists();
    void populateFiles(const QString &name);
    QStringList getAllItems(QListWidget *listWidget);
    int findItem(QListWidget *listWidget, const QString &text);
    void setCurrentItem(QListWidget *listWidget, const QString &text);
    void moveItem(QListWidget *listWidget, int from, int to);
    void itemMoveUp(QListWidget *listWidget);
    void itemMoveDown(QListWidget *listWidget);
    void addPlaylist(const QString &name);
    void removePlaylist(const QString &name);
    void renamePlaylist(const QString &oldName, const QString &newName);
    void initIcons();

private:
    Ui::PlaylistDialog *ui = nullptr;
    QString currentPlaylist;

private:
    Q_DISABLE_COPY(PlaylistDialog)
};
