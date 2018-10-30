#include "traymenu.h"
#include "ui_traymenu.h"

#include <QCursor>

TrayMenu::TrayMenu(QWidget *parent) :
    QMenu(parent),
    ui(new Ui::TrayMenu)
{
    ui->setupUi(this);
    connect(ui->toolButton_playing, &QToolButton::clicked, ui->toolButton_options, &QToolButton::clicked);
    connect(ui->toolButton_options, &QToolButton::clicked, this, &TrayMenu::onOptionsClicked);
    connect(ui->toolButton_about, &QToolButton::clicked, this, &TrayMenu::onAboutClicked);
    connect(ui->toolButton_mute, &QToolButton::clicked, this, &TrayMenu::onMuteClicked);
    connect(ui->pushButton_next, &QPushButton::clicked, this, &TrayMenu::onNextClicked);
    connect(ui->pushButton_play, &QPushButton::clicked, this, &TrayMenu::onPlayClicked);
    connect(ui->pushButton_previous, &QPushButton::clicked, this, &TrayMenu::onPreviousClicked);
    connect(ui->toolButton_exit, &QToolButton::clicked, this, &TrayMenu::onExitClicked);
}

TrayMenu::~TrayMenu()
{
    delete ui;
}

void TrayMenu::refreshTexts()
{
    ui->retranslateUi(this);
}

void TrayMenu::showEvent(QShowEvent *event)
{
    move(QCursor::pos().x() + 30, QCursor::pos().y() - height() + 30);
    QMenu::showEvent(event);
}
