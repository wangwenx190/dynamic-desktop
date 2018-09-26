#pragma once

#include "framelesswindow.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public FramelessWindow
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
    void about();

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override;

public slots:
    void setAudioAreaEnabled(bool enabled);

protected:
    void showEvent(QShowEvent *event) override;
    void changeEvent(QEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void refreshUI();
    void saveSettings();

private:
    Ui::PreferencesDialog *ui;
    bool closing = false;
};
