#include "playlistdialog.h"
#include "ui_playlistdialog.h"

PlaylistDialog::PlaylistDialog(QWidget *parent) : QWidget(parent)
{
    ui = new Ui::PlaylistDialog();
    ui->setupUi(this);
}

PlaylistDialog::~PlaylistDialog()
{
    delete ui;
}
