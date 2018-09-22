#pragma once

#include <QWidget>

class WallpaperWidget : public QWidget
{
    Q_OBJECT

public:
    WallpaperWidget(QWidget *parent = nullptr);
    ~WallpaperWidget() override;
};
