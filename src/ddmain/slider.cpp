#include "slider.h"

#include <QMouseEvent>

Slider::Slider(QWidget *parent) : QSlider(parent)
{
}

void Slider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    float currentPositionPercent = event->pos().x() / static_cast<float>(width());
    setValue(static_cast<int>(currentPositionPercent * (maximum() - minimum())) + minimum());
    emit sliderMoved(value());
}

#ifndef DD_NO_WHEEL_EVENT
void Slider::wheelEvent(QWheelEvent *event)
{
    QSlider::wheelEvent(event);
    emit sliderMoved(value());
}
#endif
