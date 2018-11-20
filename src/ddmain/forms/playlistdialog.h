#pragma once

#include <QWidget>

namespace Ui
{
    class PlaylistDialog;
}

class PlaylistDialog : public QWidget
{
    Q_OBJECT

signals:
    void playlistChanged();

public:
    explicit PlaylistDialog(QWidget *parent = nullptr);
    ~PlaylistDialog() override;

private slots:
    void populatePlaylists();

private:
    Ui::PlaylistDialog *ui = nullptr;
};
