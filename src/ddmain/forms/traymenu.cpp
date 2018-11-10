#include "traymenu.h"

#ifndef DD_NO_MENU

#include "ui_traymenu.h"

TrayMenu::TrayMenu(QWidget *parent) : QMenu(parent)
{
    ui = new Ui::TrayMenu();
    ui->setupUi(this);
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

#ifndef DD_NO_TRANSLATIONS
void TrayMenu::refreshTexts(const QString &language)
{
    Q_UNUSED(language)
    ui->retranslateUi(this);
    if (muted)
        ui->toolButton_mute->setText(DD_TR("Unmute"));
    else
        ui->toolButton_mute->setText(DD_TR("Mute"));
}
#endif

void TrayMenu::setMute(bool mute)
{
    if (muted != mute)
    {
        muted = mute;
        if (muted)
        {
            ui->toolButton_mute->setText(DD_TR("Unmute"));
            ui->toolButton_mute->setIcon(QIcon(QStringLiteral(":/icons/sound-light.svg")));
        }
        else
        {
            ui->toolButton_mute->setText(DD_TR("Mute"));
            ui->toolButton_mute->setIcon(QIcon(QStringLiteral(":/icons/mute-light.svg")));
        }
    }
}

void TrayMenu::setPlaying(bool playing)
{
    if (playing)
        ui->pushButton_play->setIcon(QIcon(QStringLiteral(":/icons/pause-light.svg")));
    else
        ui->pushButton_play->setIcon(QIcon(QStringLiteral(":/icons/play-light.svg")));
}

void TrayMenu::showEvent(QShowEvent *event)
{
    move(geometry().left() + 30, geometry().top() - height() - 30);
    QMenu::showEvent(event);
}

#endif
