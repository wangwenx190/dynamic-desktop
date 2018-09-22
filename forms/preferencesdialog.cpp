#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include <QCoreApplication>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pushButton_aboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}
