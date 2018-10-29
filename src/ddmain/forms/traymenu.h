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

public:
    explicit TrayMenu(QWidget *parent = nullptr);
    ~TrayMenu() override;

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::TrayMenu *ui;
};
