#pragma once

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QListWidget)

namespace Ui
{
    class PlaylistDialog;
}

class PlaylistDialog : public QWidget
{
    Q_OBJECT

signals:
    void dataRefreshed();

public:
    explicit PlaylistDialog(QWidget *parent = nullptr);
    ~PlaylistDialog() override;

private slots:
    void populatePlaylists();
    void populateFiles(const QString &name);
    QStringList getAllItems(QListWidget *listWidget);
    int findItem(QListWidget *listWidget, const QString &text);
    void setCurrentItem(QListWidget *listWidget, const QString &text);

private:
    Ui::PlaylistDialog *ui = nullptr;
};
