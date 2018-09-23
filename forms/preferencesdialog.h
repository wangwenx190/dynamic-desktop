#pragma once

#include <QWidget>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QWidget
{
    Q_OBJECT

signals:
    void play();
    void pause();
    void muteChanged(bool);
    void volumeChanged(unsigned int);
    void urlChanged(const QString &);
    void autostartChanged(bool);
    void refreshUi();

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override;

public slots:
    void setAudioAreaEnabled(bool enabled);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void refreshUI();
    void saveSettings();

private:
    Ui::PreferencesDialog *ui;
};
