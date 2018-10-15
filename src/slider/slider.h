#pragma once

#include "../dd_dll_global.h"

#include <QSlider>

class DD_SHARED_EXPORT Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};
