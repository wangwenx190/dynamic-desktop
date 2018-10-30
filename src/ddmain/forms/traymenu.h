#pragma once

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

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::TrayMenu *ui;
};
