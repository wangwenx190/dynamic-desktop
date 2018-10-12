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

void Slider::wheelEvent(QWheelEvent *event)
{
    QSlider::wheelEvent(event);
    emit sliderMoved(value());
}
