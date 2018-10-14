#include "framelesswindow.h"

#include <windows.h>
//#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
//#include <objidl.h>
//#include <gdiplus.h>
//#include <GdiPlusColor.h>

FramelessWindow::FramelessWindow(QWidget *parent)
    : QWidget(parent)
{
//    setWindowFlag(Qt::Window,true);
//    setWindowFlag(Qt::FramelessWindowHint, true);
//    setWindowFlag(Qt::WindowSystemMenuHint, true);
//    setWindowFlag() is not avaliable before Qt v5.9, so we should use setWindowFlags instead

    setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

    setResizeable(m_bResizeable);
}

void FramelessWindow::setResizeable(bool resizeable)
{
    bool visible = isVisible();
    m_bResizeable = resizeable;
    if (m_bResizeable){
        setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
//        setWindowFlag(Qt::WindowMaximizeButtonHint);

        //this line will get titlebar/thick frame/Aero back, which is exactly what we want
        //we will get rid of titlebar and thick frame again in nativeEvent() later
        auto hwnd = reinterpret_cast<HWND>(this->winId());
        DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    }else{
        setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
//        setWindowFlag(Qt::WindowMaximizeButtonHint,false);

        auto hwnd = reinterpret_cast<HWND>(this->winId());
        DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        ::SetWindowLong(hwnd, GWL_STYLE, style & ~WS_MAXIMIZEBOX & ~WS_CAPTION);
    }

    //we better left 1 piexl width of border untouch, so OS can draw nice shadow around it
    const MARGINS shadow = { 1, 1, 1, 1 };
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);

    setVisible(visible);
}

void FramelessWindow::setResizeableAreaWidth(int width)
{
    if (1 > width) width = 1;
    m_borderWidth = width;
}

void FramelessWindow::setTitleBar(QWidget* titlebar)
{
    m_titlebar = titlebar;
    if (!titlebar) return;
    connect(titlebar, &QWidget::destroyed, this, &FramelessWindow::onTitleBarDestroyed);
}

void FramelessWindow::onTitleBarDestroyed()
{
    if (m_titlebar == QObject::sender())
    {
        m_titlebar = Q_NULLPTR;
    }
}

void FramelessWindow::addIgnoreWidget(QWidget* widget)
{
    if (!widget) return;
    if (m_whiteList.contains(widget)) return;
    m_whiteList.append(widget);
}

bool FramelessWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    auto* msg = reinterpret_cast<MSG *>(message);
    switch (msg->message)
    {
    case WM_NCCALCSIZE:
    {
        //this kills the window frame and title bar we added with WS_THICKFRAME and WS_CAPTION
        *result = 0;
        return true;
    }
    case WM_NCHITTEST:
    {
        *result = 0;

        const LONG border_width = m_borderWidth;
        RECT winrect;
        GetWindowRect(HWND(winId()), &winrect);

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        if(m_bResizeable)
        {

            bool resizeWidth = minimumWidth() != maximumWidth();
            bool resizeHeight = minimumHeight() != maximumHeight();

            if(resizeWidth)
            {
                //left border
                if (x >= winrect.left && x < winrect.left + border_width)
                {
                    *result = HTLEFT;
                }
                //right border
                if (x < winrect.right && x >= winrect.right - border_width)
                {
                    *result = HTRIGHT;
                }
            }
            if(resizeHeight)
            {
                //bottom border
                if (y < winrect.bottom && y >= winrect.bottom - border_width)
                {
                    *result = HTBOTTOM;
                }
                //top border
                if (y >= winrect.top && y < winrect.top + border_width)
                {
                    *result = HTTOP;
                }
            }
            if(resizeWidth && resizeHeight)
            {
                //bottom left corner
                if (x >= winrect.left && x < winrect.left + border_width &&
                        y < winrect.bottom && y >= winrect.bottom - border_width)
                {
                    *result = HTBOTTOMLEFT;
                }
                //bottom right corner
                if (x < winrect.right && x >= winrect.right - border_width &&
                        y < winrect.bottom && y >= winrect.bottom - border_width)
                {
                    *result = HTBOTTOMRIGHT;
                }
                //top left corner
                if (x >= winrect.left && x < winrect.left + border_width &&
                        y >= winrect.top && y < winrect.top + border_width)
                {
                    *result = HTTOPLEFT;
                }
                //top right corner
                if (x < winrect.right && x >= winrect.right - border_width &&
                        y >= winrect.top && y < winrect.top + border_width)
                {
                    *result = HTTOPRIGHT;
                }
            }
        }
        if (0 != *result) return true;

        //*result still equals 0, that means the cursor locate OUTSIDE the frame area
        //but it may locate in titlebar area
        if (!m_titlebar) return false;

        //support highdpi
        double dpr = this->devicePixelRatioF();
        QPoint pos = m_titlebar->mapFromGlobal(QPoint(x/dpr,y/dpr));

        if (!m_titlebar->rect().contains(pos)) return false;
        QWidget* child = m_titlebar->childAt(pos);
        if (!child)
        {
            *result = HTCAPTION;
            return true;
        }
        if (m_whiteList.contains(child))
        {
            *result = HTCAPTION;
            return true;
        }
        return false;
    } //end case WM_NCHITTEST
    case WM_GETMINMAXINFO:
    {
        if (::IsZoomed(msg->hwnd)) {
            RECT frame = { 0, 0, 0, 0 };
            AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);

            //record frame area data
            double dpr = this->devicePixelRatioF();

            m_frames.setLeft(abs(frame.left)/dpr+0.5);
            m_frames.setTop(abs(frame.bottom)/dpr+0.5);
            m_frames.setRight(abs(frame.right)/dpr+0.5);
            m_frames.setBottom(abs(frame.bottom)/dpr+0.5);

            QWidget::setContentsMargins(m_frames.left()+m_margins.left(), \
                                            m_frames.top()+m_margins.top(), \
                                            m_frames.right()+m_margins.right(), \
                                            m_frames.bottom()+m_margins.bottom());
            m_bJustMaximized = true;
        }else {
            if (m_bJustMaximized)
            {
                QWidget::setContentsMargins(m_margins);
                //after window back to normal size from maximized state
                //a twinkle will happen, to avoid this twinkle
                //repaint() is important used just before the window back to normal
                repaint();
                m_frames = QMargins();
                m_bJustMaximized = false;
            }
        }
        return false;
    }
    default:
        return QWidget::nativeEvent(eventType, message, result);
    }
}

void FramelessWindow::setContentsMargins(QMargins margins)
{
    QWidget::setContentsMargins(margins+m_frames);
    m_margins = margins;
}
void FramelessWindow::setContentsMargins(int left, int top, int right, int bottom)
{
    QWidget::setContentsMargins(left+m_frames.left(),\
                                    top+m_frames.top(), \
                                    right+m_frames.right(), \
                                    bottom+m_frames.bottom());
    m_margins.setLeft(left);
    m_margins.setTop(top);
    m_margins.setRight(right);
    m_margins.setBottom(bottom);
}
QMargins FramelessWindow::contentsMargins() const
{
    QMargins margins = QWidget::contentsMargins();
    margins -= m_frames;
    return margins;
}
void FramelessWindow::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    QWidget::getContentsMargins(left,top,right,bottom);
    if (!(left&&top&&right&&bottom)) return;
    if (isMaximized())
    {
        *left -= m_frames.left();
        *top -= m_frames.top();
        *right -= m_frames.right();
        *bottom -= m_frames.bottom();
    }
}
QRect FramelessWindow::contentsRect() const
{
    QRect rect = QWidget::contentsRect();
    int width = rect.width();
    int height = rect.height();
    rect.setLeft(rect.left() - m_frames.left());
    rect.setTop(rect.top() - m_frames.top());
    rect.setWidth(width);
    rect.setHeight(height);
    return rect;
}
void FramelessWindow::showFullScreen()
{
    if (isMaximized())
    {
        QWidget::setContentsMargins(m_margins);
        m_frames = QMargins();
    }
    QWidget::showFullScreen();
}
