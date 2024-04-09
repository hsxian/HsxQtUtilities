#ifndef LINESTEGMENT_H
#define LINESTEGMENT_H

#include <QPointF>

#include <HsxQtUtilitiesCore_global.h>

namespace hsx
{
    class HSXQTUTILITIESCORE_EXPORT  LineStegment
    {
        double d_x1;
        double d_y1;
        double d_x2;
        double d_y2;
        double d_a;
        double d_b;
        double d_c;
    public:
        LineStegment(const QPointF &p1, const QPointF &p2);
        LineStegment(const double &x1, const double &y1, const double &x2, const double &y2);
        double getA();
        double getB();
        double getC();
        QPointF getFootPoint(const QPointF &p);
        double lenght();
        double closestDistance(const QPointF &p);
    };
}
#endif // LINESTEGMENT_H
