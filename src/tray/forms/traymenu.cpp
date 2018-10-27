#include "traymenu.h"
#include "ui_traymenu.h"

#include <QCursor>

TrayMenu::TrayMenu(QWidget *parent) :
    QMenu(parent),
    ui(new Ui::TrayMenu)
{
    ui->setupUi(this);
    connect(ui->toolButton_exit, &QToolButton::clicked, this, &TrayMenu::onExitClicked);
}

TrayMenu::~TrayMenu()
{
    delete ui;
}

void TrayMenu::showEvent(QShowEvent *event)
{
    move(geometry().left(), geometry().top() - height());
    QMenu::showEvent(event);
}
