#include "GoesGeomHelper.h"

#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/operation/valid/IsValidOp.h>
#include <geos/geom/LineString.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Point.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/io/WKTWriter.h>

using namespace geos::geom;
using namespace hsx;

GeosGeomHelper::GeosGeomHelper()
{

}
geos::geom::Polygon *GeosGeomHelper::toPy(geos::geom::GeometryFactory &gf, const CoordinateSequence &coords)
{
    if(coords.size() < 4)
    {
        qDebug() << __FUNCTION__ << "points size less than 4";
        return nullptr;
    }
    auto f = coords.getAt(0);
    auto l = coords.getAt(coords.size() - 1);
    if(f.x != l.x || f.y != l.y)
    {
        return nullptr;
    }
    auto csf = gf.getCoordinateSequenceFactory();
    auto cs = csf->create(coords);
    auto lr  = gf.createLinearRing(cs);
    auto ret =  gf.createPolygon(lr, NULL);
    auto iVaild = geos::operation::valid::IsValidOp::isValid(*ret);
    return  iVaild ? ret : nullptr;
}

QString GeosGeomHelper::toWkt(Geometry *g)
{
    geos::io::WKTWriter w;
    w.setOutputDimension(3);
    auto wkt = w.write(g);
    return QString::fromStdString(wkt);
}

void GeosGeomHelper::getPointsFrom(const LineString *ls, std::vector<geos::geom::Coordinate> &ret)
{
    auto count = ls->getNumPoints();

    for(std::size_t i = 0; i < count; ++i)
    {
        auto p = ls->getPointN(i);
        auto alt = p->getCoordinate()->z;
        ret.push_back(geos::geom::Coordinate(p->getX(), p->getY(), std::isnan(alt) ? 0 : alt));
    }

}

void GeosGeomHelper::getPointsFrom(const Geometry *gt, std::vector<geos::geom::Coordinate> &ret)
{
    auto cs = gt->getCoordinates();
    auto count = cs->size();
    for(std::size_t i = 0; i < count; ++i)
    {
        auto p = cs->getAt(i);
        //            qDebug() << __FUNCTION__ << p->getX() << p->getY();
        ret.push_back(geos::geom::Coordinate(p.x, p.y, 0));
    }
}

double GeosGeomHelper::length2d(const geos::geom::Coordinate &p1, const  geos::geom::Coordinate &p2)
{
    auto dx = p1.x - p2.x;
    auto dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

Coordinate GeosGeomHelper::add(const geos::geom::Coordinate &p1, const  geos::geom::Coordinate &p2)
{
    return Coordinate(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}
double GeosGeomHelper::length(const geos::geom::Coordinate &p)
{
    return qSqrt( qPow(p.x, 2) + qPow(p.y, 2) + qPow(p.z, 2));
}

Coordinate GeosGeomHelper::divide(const geos::geom::Coordinate &p, double scalar)
{
    // 需要检查 scalar 是否为零，避免除以零的情况
    if (scalar != 0)
    {
        return geos::geom::Coordinate(p.x / scalar, p.y / scalar, p.z / scalar);
    }
    else
    {
        std::cerr << "Error: Division by zero!" << std::endl;
        // 如果除数为零，返回原始坐标
        return p;
    }
}

Coordinate GeosGeomHelper::subtract(const geos::geom::Coordinate &p1, const  geos::geom::Coordinate &p2)
{
    return Coordinate(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

double GeosGeomHelper::length(const QPointF &p1, const QPointF &p2)
{
    auto dx = p1.x() - p2.x();
    auto dy = p1.y() - p2.y();
    return std::sqrt(dx * dx + dy * dy);
}

double GeosGeomHelper::crossProduct(const QPointF &a, const QPointF &b, const QPointF &c)
{
    return (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
}

bool GeosGeomHelper::isPointOnSegment(const QPointF &a, const QPointF &b, const QPointF &c)
{
    auto cross = crossProduct(a, b, c);
    auto lenAb = length(a, b);
    auto lenAc = length(a, c);
    auto lenBc = length(b, c);
    return std::abs(cross) < 1e-6 && std::abs(lenAb - (lenAc + lenBc)) < 1e-6;
}

Coordinate hsx::GeosGeomHelper::multiply(const geos::geom::Coordinate &p, double scalar)
{
    return geos::geom::Coordinate(p.x * scalar, p.y * scalar, p.z * scalar);
}
