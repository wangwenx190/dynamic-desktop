#pragma once

#include <QWidget>

namespace Ui
{
    class AboutDialog;
}

class AboutDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

public slots:
    void refreshTexts();

private:
    Ui::AboutDialog *ui;
};
