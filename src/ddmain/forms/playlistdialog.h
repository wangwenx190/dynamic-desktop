#pragma once

#include <QWidget>

namespace Ui
{
    class PlaylistDialog;
}

class PlaylistDialog : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistDialog(QWidget *parent = nullptr);
    ~PlaylistDialog() override;

private:
    Ui::PlaylistDialog *ui = nullptr;
};
