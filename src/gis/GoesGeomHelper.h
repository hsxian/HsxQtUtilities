#ifndef GOESGEOSGEOMHELPER_H
#define GOESGEOSGEOMHELPER_H

#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>
#include <vector>
#include "HsxQtUtilities_global.h"
#include <QString>
#include <QPointF>
#include <QDebug>
#include <geos/io/WKTReader.h>

namespace geos
{
namespace geom
{
class Coordinate;
class CoordinateSequence;
}
}
namespace hsx
{
/**
 * @brief The GeosGeomHelper class
 * @author hsx
 */
class HSXQTUTILITIES_EXPORT GeosGeomHelper
{
public:
    GeosGeomHelper();
    geos::geom::Polygon *toPy(geos::geom::GeometryFactory &gf, const geos::geom::CoordinateSequence &points);
    /**
     * @brief wkt转geos
     */
    template<class T>
    T *ToGeometry(const QString &wkt)
    {
        geos::io::WKTReader reader;
        //        qDebug() << __FUNCTION__ << wkt;
        return dynamic_cast<T *>(reader.read(wkt.toStdString()));
    }
    QString toWkt(geos::geom::Geometry *g);
    /**
     * @brief 二维长度
     * @param p1
     * @param p2
     * @return
     */
    static double length2d(const geos::geom::Coordinate &p1, const  geos::geom::Coordinate &p2);
    static geos::geom::Coordinate add(const geos::geom::Coordinate &p1, const geos::geom::Coordinate &p2);
    static geos::geom::Coordinate subtract(const geos::geom::Coordinate &p1, const geos::geom::Coordinate &p2);
    static geos::geom::Coordinate multiply(const geos::geom::Coordinate &p, double scalar);
    static geos::geom::Coordinate divide(const geos::geom::Coordinate &p, double scalar);
    static double length(const geos::geom::Coordinate &p);
    double length(const QPointF &p1, const QPointF &p2);
    double crossProduct(const QPointF &a, const QPointF &b, const QPointF &c);
    bool isPointOnSegment(const QPointF &a, const QPointF &b, const QPointF &c);
    void getPointsFrom(const geos::geom::LineString *ls, std::vector<geos::geom::Coordinate> &ret);
    void getPointsFrom(const geos::geom::Geometry *gt, std::vector<geos::geom::Coordinate> &ret);
};
}
#endif // GOESGEOSGEOMHELPER_H
