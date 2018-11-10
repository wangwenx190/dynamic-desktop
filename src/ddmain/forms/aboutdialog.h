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

#ifndef DD_NO_TRANSLATIONS
public slots:
    void refreshTexts(const QString &language);
#endif

private:
    Ui::AboutDialog *ui = nullptr;
};
