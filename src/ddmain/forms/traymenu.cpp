#include "traymenu.h"

#ifndef DD_NO_MENU

#include "ui_traymenu.h"

TrayMenu::TrayMenu(QWidget *parent) : QMenu(parent)
{
    ui = new Ui::TrayMenu();
    ui->setupUi(this);
    initIcons();
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
#ifndef DD_NO_SVG
            ui->toolButton_mute->setIcon(QIcon(QStringLiteral(":/icons/sound-light.svg")));
#else
            ui->toolButton_mute->setIcon(QIcon(QStringLiteral(":/icons/sound-light.png")));
#endif
        }
        else
        {
            ui->toolButton_mute->setText(DD_TR("Mute"));
#ifndef DD_NO_SVG
            ui->toolButton_mute->setIcon(QIcon(QStringLiteral(":/icons/mute-light.svg")));
#else
            ui->toolButton_mute->setIcon(QIcon(QStringLiteral(":/icons/mute-light.png")));
#endif
        }
    }
}

void TrayMenu::setPlaying(bool playing)
{
    if (playing)
#ifndef DD_NO_SVG
        ui->pushButton_play->setIcon(QIcon(QStringLiteral(":/icons/pause-light.svg")));
#else
        ui->pushButton_play->setIcon(QIcon(QStringLiteral(":/icons/pause-light.png")));
#endif
    else
#ifndef DD_NO_SVG
        ui->pushButton_play->setIcon(QIcon(QStringLiteral(":/icons/play-light.svg")));
#else
        ui->pushButton_play->setIcon(QIcon(QStringLiteral(":/icons/play-light.png")));
#endif
}

void TrayMenu::showEvent(QShowEvent *event)
{
    move(geometry().left(), geometry().top() - height());
    QMenu::showEvent(event);
}

void TrayMenu::initIcons()
{
#ifndef DD_NO_SVG
    ui->pushButton_previous->setIcon(QIcon(QStringLiteral(":/icons/previous-light.svg")));
    ui->pushButton_play->setIcon(QIcon(QStringLiteral(":/icons/play-light.svg")));
    ui->pushButton_next->setIcon(QIcon(QStringLiteral(":/icons/next-light.svg")));
    ui->toolButton_playlist->setIcon(QIcon(QStringLiteral(":/icons/playlist-light.svg")));
    ui->toolButton_playback_mode->setIcon(QIcon(QStringLiteral(":/icons/circle-light.svg")));
    ui->toolButton_mute->setIcon(QIcon(QStringLiteral(":/icons/mute-light.svg")));
    ui->toolButton_options->setIcon(QIcon(QStringLiteral(":/icons/options-light.svg")));
    ui->toolButton_about->setIcon(QIcon(QStringLiteral(":/icons/info.svg")));
    ui->toolButton_exit->setIcon(QIcon(QStringLiteral(":/icons/exit-light.svg")));
#else
    ui->pushButton_previous->setIcon(QIcon(QStringLiteral(":/icons/previous-light.png")));
    ui->pushButton_play->setIcon(QIcon(QStringLiteral(":/icons/play-light.png")));
    ui->pushButton_next->setIcon(QIcon(QStringLiteral(":/icons/next-light.png")));
    ui->toolButton_playlist->setIcon(QIcon(QStringLiteral(":/icons/playlist-light.png")));
    ui->toolButton_playback_mode->setIcon(QIcon(QStringLiteral(":/icons/circle-light.png")));
    ui->toolButton_mute->setIcon(QIcon(QStringLiteral(":/icons/mute-light.png")));
    ui->toolButton_options->setIcon(QIcon(QStringLiteral(":/icons/options-light.png")));
    ui->toolButton_about->setIcon(QIcon(QStringLiteral(":/icons/info.png")));
    ui->toolButton_exit->setIcon(QIcon(QStringLiteral(":/icons/exit-light.png")));
#endif
}

#endif
