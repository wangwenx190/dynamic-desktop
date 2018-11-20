#pragma once

#ifndef DD_NO_MENU

#include <QMenu>

namespace Ui
{
    class TrayMenu;
}

class TrayMenu : public QMenu
{
    Q_OBJECT

signals:
    void onExitClicked();
    void onOptionsClicked();
    void onPlayClicked();
    void onNextClicked();
    void onPreviousClicked();
    void onMuteClicked();
    void onAboutClicked();

public:
    explicit TrayMenu(QWidget *parent = nullptr);
    ~TrayMenu() override;

public slots:
#ifndef DD_NO_TRANSLATIONS
    void refreshTexts(const QString &language);
#endif
    void setMute(bool mute = true);
    void setPlaying(bool playing = true);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void initIcons();

private:
    Ui::TrayMenu *ui = nullptr;
    bool muted = false;
};

#endif
