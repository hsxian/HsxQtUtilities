#ifndef WINDOWMOVEHELPER_H
#define WINDOWMOVEHELPER_H

#include <QObject>
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>

#include <HsxQtUtilitiesCore_global.h>

namespace hsx
{
    class HSXQTUTILITIESCORE_EXPORT  WindowMoveHelper: protected QObject
    {
        Q_OBJECT
    public:
        WindowMoveHelper(QWidget *win);
        void resetEnableMouseMoveEvent(QMouseEvent *event);
    private:
        QWidget *_win = 0;
        bool _canMove = false;

        QPoint _mouseStartPoint;
        QPoint _winPoint;

        // QObject interface
    public:
        bool eventFilter(QObject *watched, QEvent *event);
    };
}
#endif // WINDOWMOVEHELPER_H
