#pragma once

#include <QSlider>

class Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *event) override;
#ifndef DD_NO_WHEEL_EVENT
    void wheelEvent(QWheelEvent *event) override;
#endif
};
