#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}
