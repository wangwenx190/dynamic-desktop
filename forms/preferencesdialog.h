#pragma once

#include <QWidget>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override;

private:
    Ui::PreferencesDialog *ui;
};
