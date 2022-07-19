#include "LineStegment.h"

#include <QtMath>

hsx::LineStegment::LineStegment(const double &x1, const double &y1, const double &x2, const double &y2)
{
    d_x1 = x1;
    d_x2 = x2;
    d_y1 = y1;
    d_y2 = y2;
    d_a = d_y2 - d_y1;
    d_b = d_x1 - d_x2;
    d_c = d_x2 * d_y1 - d_x1 * d_y2;
}

hsx::LineStegment::LineStegment(const QPointF &p1, const QPointF &p2)
{
    d_x1 = p1.x();
    d_x2 = p2.x();
    d_y1 = p1.y();
    d_y2 = p2.y();
    d_a = d_y2 - d_y1;
    d_b = d_x1 - d_x2;
    d_c = d_x2 * d_y1 - d_x1 * d_y2;
}

double hsx::LineStegment::getA()
{
    return d_a;
}

double hsx::LineStegment::getB()
{
    return d_b;
}

double hsx::LineStegment::getC()
{
    return d_c;
}

QPointF hsx::LineStegment::getFootPoint(const QPointF &p)
{
    auto ab2 = d_a * d_a + d_b * d_b;
    auto x = ((d_b * d_b * p.x() - d_a * d_b * p.y() - d_a * d_c) / ab2);
    auto y = ((d_a * d_a * p.y() - d_a * d_b * p.x() - d_b * d_c) / ab2);
    return QPointF(x, y);
}

double hsx::LineStegment::lenght()
{
    auto dx = d_x1 - d_x2;
    auto dy = d_y1 - d_y2;
    return qSqrt(dx * dx + dy * dy);
}

double hsx::LineStegment::closestDistance(const QPointF &p)
{
    auto cross = (d_x2 - d_x1) * (p.x() - d_x1) + (d_y2 - d_y1) * (p.y() - d_y1);
    if(cross < 0)
    {
        return qSqrt((p.x() - d_x1) * (p.x() - d_x1) + (p.y() - d_y1) * (p.y() - d_y1));
    }
    double d2 = (d_x2 - d_x1) * (d_x2 - d_x1) + (d_y2 - d_y1) * (d_y2 - d_y1);
    if(cross >= d2)
    {
        return qSqrt((p.x() - d_x2) * (p.x() - d_x2) + (p.y() - d_y2) * (p.y() - d_y2));
    }
    auto r = cross / d2;
    auto px = d_x1 + (d_x2 - d_x1) * r;
    auto py = d_y1 + (d_y2 - d_y1) * r;
    return qSqrt((p.x() - px) * (p.x() - px) + (py - d_y1) * (py - d_y1));
}
