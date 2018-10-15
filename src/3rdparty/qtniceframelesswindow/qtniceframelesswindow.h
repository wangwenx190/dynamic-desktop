#pragma once

#include "../dd_dll_global.h"

#include <QWidget>

//A nice frameless window for both Windows and OS X
//Author: Bringer-of-Light
//Github: https://github.com/Bringer-of-Light/Qt-Nice-Frameless-Window
// Usage: use "FramelessWindow" as base class instead of "QWidget", and enjoy

class DD_SHARED_EXPORT FramelessWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FramelessWindow(QWidget *parent = nullptr);
public:

    //if resizeable is set to false, then the window can not be resized by mouse
    //but still can be resized programtically
    void setResizeable(bool resizeable=true);
    bool isResizeable(){return m_bResizeable;}

    //set border width, inside this aera, window can be resized by mouse
    void setResizeableAreaWidth(int width = 5);
protected:
    //set a widget which will be treat as SYSTEM titlebar
    void setTitleBar(QWidget* titlebar);

    //generally, we can add widget say "label1" on titlebar, and it will cover the titlebar under it
    //as a result, we can not drag and move the MainWindow with this "label1" again
    //we can fix this by add "label1" to a ignorelist, just call addIgnoreWidget(label1)
    void addIgnoreWidget(QWidget* widget);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
private slots:
    void onTitleBarDestroyed();
public:
    void setContentsMargins(QMargins margins);
    void setContentsMargins(int left, int top, int right, int bottom);
    QMargins contentsMargins() const;
    QRect contentsRect() const;
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
public slots:
    void showFullScreen();
private:
    QWidget* m_titlebar = nullptr;
    QList<QWidget*> m_whiteList;
    int m_borderWidth = 5;

    QMargins m_margins = QMargins();
    QMargins m_frames = QMargins();
    bool m_bJustMaximized = false;

    bool m_bResizeable = true;
};
