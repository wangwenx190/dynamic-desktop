#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include <QApplication>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_ok, &QPushButton::clicked, qApp, &QApplication::closeAllWindows);
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pushButton_aboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}
