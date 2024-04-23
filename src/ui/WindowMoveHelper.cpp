#include "WindowMoveHelper.h"
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>

using namespace hsx;

WindowMoveHelper::WindowMoveHelper(QWidget *win)
{
    _win = win;
    _win->installEventFilter(this);
}

void WindowMoveHelper::resetEnableMouseMoveEvent(QMouseEvent *event)
{
    _canMove = true;
    _winPoint = _win->pos();
    _mouseStartPoint = event->globalPos();
}

bool WindowMoveHelper::eventFilter(QObject *watched, QEvent *event)
{
    if(_canMove)
    {
        auto mouseEvevnt = (QMouseEvent *)event;
        if(mouseEvevnt)
        {
            if(event->type() == QEvent::MouseMove)
            {
                if(_canMove)
                {
                    auto distance = mouseEvevnt->globalPos() - _mouseStartPoint;
                    _win->move(_winPoint + distance);
                }
            }
            else
                if(event->type() == QEvent::MouseButtonRelease)
                {
                    _canMove = false;
                }
        }
    }
    return false;
}

