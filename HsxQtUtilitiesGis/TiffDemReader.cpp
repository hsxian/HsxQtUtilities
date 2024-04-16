#include "TiffDemReader.h"


#include "GoesGeomHelper.h"
#include "TiffBox.h"

#include <gdal.h>
#include <gdal_priv.h>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <iomanip>
#include <gdal_alg.h>
#include <ogrsf_frmts.h>
#include <geos/geom/LineString.h>

#define OpenDebugLogInfoForFindClosePoint 0
using namespace hsx;
TiffDemReader::TiffDemReader()
{
}


TiffDemReader::~TiffDemReader()
{
    Close();
}

namespace hsxTiffDemReader
{
class MyGDALContourWriterData
{
public:
    TiffDemReader *tdr;
    int xOffset;
    int yOffset;
    std::vector<DemContour *> *pointss;
    float lonMin, latMin, lonMax, latMax;
};
geos::geom::Coordinate getSameSide(std::vector<geos::geom::Coordinate> &box, geos::geom::Coordinate &pReference, geos::geom::Coordinate &p)
{
    foreach(auto &tmp, box)
    {
        if(pReference.x == tmp.x)
        {
            if(pReference.y - p.y > 0 && pReference.y - tmp.y > 0)
            {
                return geos::geom::Coordinate(tmp);
            }
        }
        if(pReference.y == tmp.y)
        {
            if(pReference.x - p.x > 0 && pReference.x - tmp.x > 0)
            {
                return geos::geom::Coordinate(tmp);
            }
        }
    }
    return geos::geom::Coordinate();
}
geos::geom::Coordinate getSameSide(TiffDemReader &tdr, double interval, std::vector<geos::geom::Coordinate> &box, geos::geom::Coordinate &p)
{
    auto demP = tdr.ReadElevationCoord(p.x, p.y);
    for(size_t i = 0; i < 4; ++i)
    {
        auto &boxp = box.at(i);
        auto cp = p;
        if(p.x == boxp.x)
        {
            cp = GeosGeomHelper::subtract(cp, geos::geom::Coordinate(0, interval * 2));
        }
        else if(p.y == boxp.y)
        {
            cp = GeosGeomHelper::subtract(cp, geos::geom::Coordinate(interval * 2, 0));
        }
        auto demCp = tdr.ReadElevationCoord(cp.x, cp.y);

        if(demP < demCp)
        {
            qDebug() << __FUNCTION__ << "error";
            return getSameSide(box, p, cp);
        }
    }

    return geos::geom::Coordinate();
}
void generateBoxContours(TiffDemReader &tdr, float minLon, float minLat, float maxLon, float maxLat, std::vector<DemContour *> &ret)
{
    //bottom
    auto contourB = new DemContour();
    contourB->isBox = true;
    contourB->isValid = true;
    contourB->hadClose = false;
    auto dem = tdr.ReadElevationCoord(minLon, minLat);
    geos::geom::Coordinate bl(minLon, minLat, dem);
    contourB->points.push_back(bl);
    tdr.ReadElevationCoord(maxLon, minLat);
    geos::geom::Coordinate br(maxLon, minLat, dem);
    contourB->points.push_back(br);
    ret.push_back(contourB);
    //right
    auto contourR = new DemContour();
    contourR->isBox = true;
    contourR->isValid = true;
    contourR->hadClose = false;
    contourR->points.push_back(br);
    dem = tdr.ReadElevationCoord(maxLon, maxLat);
    geos::geom::Coordinate ur(maxLon, maxLat, dem);
    contourR->points.push_back(ur);
    ret.push_back(contourR);
    //up
    auto contourU = new DemContour();
    contourU->isBox = true;
    contourU->isValid = true;
    contourU->hadClose = false;
    contourU->points.push_back(ur);
    dem = tdr.ReadElevationCoord(minLon, maxLat);
    geos::geom::Coordinate ul(minLon, maxLat, dem);
    contourU->points.push_back(ul);
    ret.push_back(contourU);
    //left
    auto contourL = new DemContour();
    contourL->isBox = true;
    contourL->isValid = true;
    contourL->hadClose = false;
    contourL->points.push_back(ul);
    contourL->points.push_back(bl);
    ret.push_back(contourL);
}
void appendPoint(std::vector<geos::geom::Coordinate> &des, const std::vector<geos::geom::Coordinate> &sou, bool invertedOrder)
{
    auto count = sou.size();
    auto count_1 = count - 1;
    for(size_t i = 0; i < count; ++i)
    {
        auto idx = invertedOrder ? count_1 - i : i;
        des.push_back(sou.at(idx));
    }
}
enum FindCloseOrientation
{
    leftUp,
    leftDown,
    rightUp,
    rightDown,
    upLeft,
    upRight,
    downLeft,
    downRight
};
QString getFindCloseOrientationName(FindCloseOrientation ori)
{
    switch(ori)
    {
    case FindCloseOrientation::leftUp:
        return "leftUp";
    case FindCloseOrientation::leftDown:
        return "leftDown";
    case FindCloseOrientation::rightUp:
        return "rightUp";
    case FindCloseOrientation::rightDown:
        return "rightDown";
    case FindCloseOrientation::upLeft:
        return "upLeft";
    case FindCloseOrientation::upRight:
        return "upRight";
    case FindCloseOrientation::downLeft:
        return "downLeft";
    case FindCloseOrientation::downRight:
        return "downRight";
    }
}
bool isSameOrientation(geos::geom::Coordinate &oriP, geos::geom::Coordinate &p, float minLon, float minLat, float maxLon, float maxLat, double interval, FindCloseOrientation ori)
{
    switch(ori)
    {
    case FindCloseOrientation::leftUp:
        return std::abs(p.x - minLon) < interval && p.y - oriP.y > 0;
    case FindCloseOrientation::leftDown:
        return std::abs(p.x - minLon) < interval && p.y - oriP.y < 0;
    case FindCloseOrientation::rightUp:
        return std::abs(p.x - maxLon) < interval && p.y - oriP.y > 0;
    case FindCloseOrientation::rightDown:
        return std::abs(p.x - maxLon) < interval && p.y - oriP.y < 0;
    case FindCloseOrientation::upLeft:
        return std::abs(p.y - maxLat) < interval && p.x - oriP.x < 0;
    case FindCloseOrientation::upRight:
        return std::abs(p.y - maxLat) < interval && p.x - oriP.x > 0;
    case FindCloseOrientation::downLeft:
        return std::abs(p.y - minLat) < interval && p.x - oriP.x < 0;
    case FindCloseOrientation::downRight:
        return std::abs(p.y - minLat) < interval && p.x - oriP.x > 0;
    }
    return false;
}
bool boxLineVerticality(DemContour *contour, FindCloseOrientation ori)
{
    if(contour->isBox == false)
    {
        return true;
    }
    auto ps = contour->points;
    switch(ori)
    {
    case FindCloseOrientation::leftUp:
        return ps.at(0).x != ps.at(1).x;
    case FindCloseOrientation::leftDown:
        return ps.at(0).x != ps.at(1).x;
    case FindCloseOrientation::rightUp:
        return ps.at(0).x != ps.at(1).x;
    case FindCloseOrientation::rightDown:
        return ps.at(0).x != ps.at(1).x;
    case FindCloseOrientation::upLeft:
        return ps.at(0).y != ps.at(1).y;
    case FindCloseOrientation::upRight:
        return ps.at(0).y != ps.at(1).y;
    case FindCloseOrientation::downLeft:
        return ps.at(0).y != ps.at(1).y;
    case FindCloseOrientation::downRight:
        return ps.at(0).y != ps.at(1).y;
    }
    return false;
}
void findClosePointNextPoint(DemContour *currContour, std::vector<DemContour *> &contours, geos::geom::Coordinate &currPoint,
                             float minLon, float minLat, float maxLon, float maxLat, double interval, FindCloseOrientation ori, bool &continueFind, geos::geom::Coordinate &nextP)
{
    int minDIdx = -1;
    double minD = std::numeric_limits<double>().max();
    bool invertedOrder = false;
    geos::geom::Coordinate currMinDistancePoint;
    for(size_t i = 0; i < contours.size(); ++i)
    {
        auto cr = contours.at(i);
        if(cr->isValid == false || cr->hadClose || (currContour->dem != cr->dem && cr->isBox == false))
        {
            continue;
        }
        auto crfp = cr->points.at(0);
        auto crlp = cr->points.at(cr->points.size() - 1);
        auto boxCan = boxLineVerticality(cr, ori);
        if(isSameOrientation(currPoint, crfp, minLon, minLat, maxLon, maxLat, interval, ori))
        {
            if(boxCan == false)
            {
                continue;
            }
            auto d = GeosGeomHelper::length2d(crfp, currPoint);
            if(minD > d)
            {
                minDIdx = i;
                minD = d;
                invertedOrder = false;
                currMinDistancePoint = crfp;
            }
#if OpenDebugLogInfoForFindClosePoint
            qDebug() << __FUNCTION__ << "crfp:" << crfp.x << crfp.y
                     << "d" << d;
#endif
        }
        if(isSameOrientation(currPoint, crlp, minLon, minLat, maxLon, maxLat, interval, ori))
        {
            if(boxCan == false)
            {
                continue;
            }
            auto d = GeosGeomHelper::length2d(crlp, currPoint);
            if(minD > d)
            {
                minDIdx = i;
                minD = d;
                invertedOrder = true;
                currMinDistancePoint = crlp;
            }
#if OpenDebugLogInfoForFindClosePoint
            qDebug() << __FUNCTION__ << "crlp:" << crlp.x << crlp.y
                     << "d" << d;
#endif
        }
    }
    if(minDIdx != -1)
    {
        auto attach = contours.at(minDIdx);
        if(invertedOrder)
        {
            auto p = attach->points.at(0);
            nextP = p;
        }
        else
        {
            auto &p = attach->points.at(attach->points.size() - 1);
            nextP = p;
        }
        if(attach == currContour)
        {
            currContour->pointsOfClose.push_back(attach->points.at(0));
            continueFind = false;
            currContour->hadClose = true;
#if OpenDebugLogInfoForFindClosePoint
            qDebug() << __FUNCTION__ << "close point:" << cr->points.at(0).x << cr->points.at(0).y;
#endif
        }
        else
        {
            if(attach->isBox)
            {
                currContour->pointsOfClose.push_back(currMinDistancePoint);
                nextP = currMinDistancePoint;
            }
            else
            {
                appendPoint(currContour->pointsOfClose, attach->points, invertedOrder);
            }
        }

        if(attach->isBox == false)
        {
            attach->isValid = false;
        }
    }
    else
    {
        continueFind = false;
    }
}

int shouldUpOrDwon(TiffDemReader &tdr, geos::geom::Coordinate &p, float pDem, double interval, int tryFindCount)
{
    for(int i = 1; i <= tryFindCount; ++i)
    {
        auto scaleInterval = interval * i * i;
        auto upDem =  tdr.ReadElevationCoord(p.x, p.y + scaleInterval);
        auto downDem =  tdr.ReadElevationCoord(p.x, p.y - scaleInterval);

        if(upDem >= pDem)
        {
            return 1;
        }
        if(downDem >= pDem)
        {
            return -1;
        }
    }
    return 0;
}
int shouldRightOrLeft(TiffDemReader &tdr, geos::geom::Coordinate &p, float pDem, double interval, int tryFindCount)
{
    for(int i = 1; i <= tryFindCount; ++i)
    {
        auto scaleInterval = interval * i * i;
        auto leftDem =  tdr.ReadElevationCoord(p.x - scaleInterval, p.y);
        auto rightDem =  tdr.ReadElevationCoord(p.x + scaleInterval, p.y);
        if(rightDem >= pDem)
        {
            return 1;
        }
        if(leftDem >= pDem)
        {
            return -1;
        }
    }
    return 0;
}
bool getOrientation(TiffDemReader &tdr, geos::geom::Coordinate &p,
                    float minLon, float minLat, float maxLon, float maxLat, double interval, FindCloseOrientation oriBefore, FindCloseOrientation &ori)
{
    auto pDem = tdr.ReadElevationCoord(p.x, p.y);

    auto dxMin = std::abs(p.x - minLon);
    auto dxMax = std::abs(p.x - maxLon);
    auto dyMin = std::abs(p.y - minLat);
    auto dyMax = std::abs(p.y - maxLat);
    if(dxMin < interval)//left
    {
        if(dyMin < interval)//left-bottom
        {
            if(oriBefore == FindCloseOrientation::leftDown)
            {
                ori = FindCloseOrientation::downRight;
                return true;
            }
            if(oriBefore == FindCloseOrientation::downLeft)
            {
                ori = FindCloseOrientation::leftUp;
                return true;
            }
        }
        if(dyMax < interval)//left-up
        {
            if(oriBefore == FindCloseOrientation::leftUp)
            {
                ori = FindCloseOrientation::upRight;
                return true;
            }
            if(oriBefore == FindCloseOrientation::upLeft)
            {
                ori = FindCloseOrientation::leftDown;
                return true;
            }
        }
        auto upOrDown = shouldUpOrDwon(tdr, p, pDem, interval, 10);
        if(upOrDown == 1)
        {
            ori = FindCloseOrientation::leftUp;
            return true;
        }
        if(upOrDown == -1)
        {
            ori = FindCloseOrientation::leftDown;
            return true;
        }

    }
    if(dxMax < interval)//right
    {
        if(dyMin < interval)//right-bottom
        {
            if(oriBefore == FindCloseOrientation::rightDown)
            {
                ori = FindCloseOrientation::downLeft;
                return true;
            }
            if(oriBefore == FindCloseOrientation::downRight)
            {
                ori = FindCloseOrientation::rightUp;
                return true;
            }
        }
        if(dyMax < interval)//right-up
        {
            if(oriBefore == FindCloseOrientation::rightUp)
            {
                ori = FindCloseOrientation::upLeft;
                return true;
            }
            if(oriBefore == FindCloseOrientation::upRight)
            {
                ori = FindCloseOrientation::rightDown;
                return true;
            }
        }
        auto upOrDown = shouldUpOrDwon(tdr, p, pDem, interval, 10);
        if(upOrDown == 1)
        {
            ori = FindCloseOrientation::rightUp;
            return true;
        }
        if(upOrDown == -1)
        {
            ori = FindCloseOrientation::rightDown;
            return true;
        }
    }
    if(dyMax < interval)//up
    {
        if(dxMin < interval)//up-left
        {
            if(oriBefore == FindCloseOrientation::upLeft)
            {
                ori = FindCloseOrientation::leftDown;
                return true;
            }
            if(oriBefore == FindCloseOrientation::leftUp)
            {
                ori = FindCloseOrientation::upRight;
                return true;
            }
        }
        if(dxMax < interval)//up-right
        {
            if(oriBefore == FindCloseOrientation::upRight)
            {
                ori = FindCloseOrientation::rightDown;
                return true;
            }
            if(oriBefore == FindCloseOrientation::rightUp)
            {
                ori = FindCloseOrientation::upLeft;
                return true;
            }
        }
        auto rightOrLeft = shouldRightOrLeft(tdr, p, pDem, interval, 10);
        if(rightOrLeft == -1)
        {
            ori = FindCloseOrientation::upLeft;
            return true;
        }
        if(rightOrLeft == 1)
        {
            ori = FindCloseOrientation::upRight;
            return true;
        }
    }
    if(dyMin < interval)//bottom
    {
        if(dxMax < interval)//bottom-right
        {
            if(oriBefore == FindCloseOrientation::downRight)
            {
                ori = FindCloseOrientation::rightUp;
                return true;
            }
            if(oriBefore == FindCloseOrientation::rightDown)
            {
                ori = FindCloseOrientation::downLeft;
                return true;
            }
        }
        if(dxMin < interval)//bottom-left
        {
            if(oriBefore == FindCloseOrientation::downLeft)
            {
                ori = FindCloseOrientation::leftUp;
                return true;
            }
            if(oriBefore == FindCloseOrientation::leftDown)
            {
                ori = FindCloseOrientation::downRight;
                return true;
            }
        }
        auto rightOrLeft = shouldRightOrLeft(tdr, p, pDem, interval, 10);
        if(rightOrLeft == -1)
        {
            ori = FindCloseOrientation::downLeft;
            return true;
        }
        if(rightOrLeft == 1)
        {
            ori = FindCloseOrientation::downRight;
            return true;
        }
    }
    return  false;
}
void findClosePoint(TiffDemReader &tdr, DemContour *currContour, geos::geom::Coordinate &currPoint, std::vector<DemContour *> &contours,
                    float minLon, float minLat, float maxLon, float maxLat, double interval, FindCloseOrientation oriBefore)
{
    FindCloseOrientation ori;
    if(getOrientation(tdr, currPoint, minLon, minLat, maxLon, maxLat, interval, oriBefore, ori))
    {
        geos::geom::Coordinate nextP;
        bool continueFind = true;
#if OpenDebugLogInfoForFindClosePoint
        qDebug() << __FUNCTION__
                 << "address:" << currContour
                 << "dem:" << currContour->dem
                 << "currPoint:" << currPoint.x << currPoint.y
                 << "oriBefore:" << getFindCloseOrientationName(oriBefore)
                 << "ori:" << getFindCloseOrientationName(ori)
                 << "count:" << currContour->pointsOfClose.size();
#endif
        findClosePointNextPoint(currContour, contours, currPoint, minLon, minLat, maxLon, maxLat, interval, ori, continueFind, nextP);
#if OpenDebugLogInfoForFindClosePoint
        qDebug() << __FUNCTION__
                 << "address:" << currContour
                 << "nextP:" << nextP.x << nextP.y
                 << "count:" << currContour->pointsOfClose.size()
                 << "continueFind" << continueFind;
#endif
        if(continueFind)
        {
            findClosePoint(tdr, currContour, nextP, contours, minLon, minLat, maxLon, maxLat, interval, ori);
        }
    }
}
CPLErr MyGDALContourWriter(double dfLevel, int nPoints,
                           double *padfX, double *padfY, void *pCBData)
{
    //    qDebug() << __FUNCTION__ << "dfLevel:" << dfLevel << "nPoints:" << nPoints;
    auto cw = (MyGDALContourWriterData *)pCBData;
    if(nPoints)
    {
        auto dc = new DemContour();
        dc->dem = dfLevel;
        float lon, lat;

        for(int i = 0; i < nPoints; i++)
        {
            auto x = cw->xOffset + *padfX;
            auto y = cw->yOffset + *padfY;
            cw->tdr->ToCoordDb(x,  y, lon, lat);
            //            qDebug() << lon << lat << dfLevel;
            dc->points.push_back(geos::geom::Coordinate(lon, lat, dfLevel));
            padfX++;
            padfY++;
        }

        cw->tdr->CloseDemContour(dc, cw->tdr->getStepX() * 1.5);
        cw->pointss->push_back(dc);
    }

    return CE_None;
}
}

using namespace hsxTiffDemReader;

void TiffDemReader::CloseDemContour(DemContour *contour, double interval)
{
    if(contour->hadClose || contour->isValid == false)
    {
        return;
    }

    auto p = contour->points.at(0);
    auto fp = geos::geom::Coordinate(p.x, p.y);
    p = contour->points.at(contour->points.size() - 1);
    auto lp = geos::geom::Coordinate(p.x, p.y);
    auto diff = GeosGeomHelper::subtract(fp, lp);
    if(GeosGeomHelper::length(diff) == 0)
    {
        contour->hadClose = true;
        return;
    };
    //    return;
    auto diffX = std::abs(fp.x - lp.x);
    auto diffY = std::abs(fp.y - lp.y);
    if(diffX < interval || diffY < interval) //one side
    {
        auto cp = GeosGeomHelper::divide( GeosGeomHelper::add(fp, lp), 2);
        auto dem = ReadElevationCoord(cp.x, cp.y);
        if(dem >= contour->dem)
        {
            contour->hadClose = true;
            contour->pointsOfClose.push_back(contour->points.at(0));
        }
    }
}

bool TiffDemReader::isValid()
{
    return valid;
}

void TiffDemReader::ContourGenerate(float sLon, float sLat, float eLon, float eLat, float enlargeScale,
                                    double dfContourInterval, double dfContourBase, bool isCloseContour, std::vector<DemContour *> &ret)
{
    ret.clear();
    //起始点和终点
    int xs, xe, ys, ye;
    ToXY(sLon, sLat, xs, ys);
    ToXY(eLon, eLat, xe, ye);

    auto xMin = std::min(xs, xe);
    auto xMax = std::max(xs, xe);
    auto yMin  = std::min(ys, ye);
    auto yMax  = std::max(ys, ye);
    auto xSize = xMax - xMin;
    auto ySize = yMax - yMin;

    TiffBox tb;
    tb.InitFromXY(*this, xs, ys, xe, ye, true);
    tb.Enlarge(*this, enlargeScale);
    //    ContourGenerateXY(tb.xMinEx, tb.yMinEx, tb.xSizeEx, tb.ySizeEx, dfContourInterval, dfContourBase, ret);
    ContourGenerateXY(xMin, yMin, xSize, ySize, dfContourInterval, dfContourBase, ret);

    double minLon, minLat, maxLon, maxLat;
    minLon = minLat = std::numeric_limits<double>().max();
    maxLon = maxLat = std::numeric_limits<double>().min();
    minLon = std::min(tb.lonMinEx, tb.lonMaxEx);
    maxLon = std::max(tb.lonMinEx, tb.lonMaxEx);
    minLat = std::min(tb.latMinEx, tb.latMaxEx);
    maxLat = std::max(tb.latMinEx, tb.latMaxEx);
    generateBoxContours(*this, minLon, minLat, maxLon, maxLat, ret);
    if(isCloseContour)
    {
        foreach(auto &contour, ret)
        {
            if(contour->isBox || contour->isValid == false || contour->hadClose)
            {
                continue;
            }
            auto p = contour->points.at(contour->points.size() - 1);
            findClosePoint(*this, contour, p, ret, minLon, minLat, maxLon, maxLat, getStepX() * 1.5, FindCloseOrientation::leftUp);
        }
    }
}

void TiffDemReader::ContourGenerateXY(int x, int y, int width, int height, double dfContourInterval, double dfContourBase, std::vector<DemContour *> &ret)
{
    auto buffer = ReadElevation(x, y, width, height);

    if(buffer)
    {
        MyGDALContourWriterData pCBData;
        pCBData.tdr = this;
        pCBData.xOffset = x;
        pCBData.yOffset = y;
        pCBData.pointss = &ret;
        float lonMin, latMin, lonMax, latMax;
        ToCoord(x, y, lonMin, latMin);
        ToCoord(x + width, y + height, lonMax, latMax);
        pCBData.lonMin = std::min(lonMin, lonMax);
        pCBData.lonMax = std::max(lonMin, lonMax);
        pCBData.latMin = std::min(latMin, latMax);
        pCBData.latMax = std::max(latMin, latMax);

        auto cg = GDAL_CG_Create(width, height, false, 0,  dfContourInterval, dfContourBase, MyGDALContourWriter, &pCBData);
        auto raw = buffer;
        auto count = width * height;
        double *bufferDB = (double *)CPLMalloc(sizeof(double) * count);
        auto line = bufferDB;
        for(int i = 0; i < count; i++)
        {
            *line = *raw;
            raw++;
            line++;
        }
        line = bufferDB;
        for(int i = 0; i < height; i++)
        {
            GDAL_CG_FeedLine(cg, line);
            line += width;
        }
        GDAL_CG_Destroy(cg);
        CPLFree(buffer);
        CPLFree(bufferDB);
        //        tryConcateTowContour(ret, lonMin, latMin, lonMax, latMax, getStepX() * 2);
    }
}
TFR_BYTE *TiffDemReader::ReadElevation(int nDSXOff, int nDSYOff, int nDSXSize, int nDSYSize)
{
    auto buffer = (TFR_BYTE *)CPLMalloc(sizeof(TFR_BYTE) * nDSXSize * nDSYSize);
    if(GDALRasterIO(hBand, GF_Read, nDSXOff, nDSYOff, nDSXSize, nDSYSize, buffer, nDSXSize, nDSYSize, GDT_Float32, 0, 0) != CE_None)
    {
        CPLFree(buffer);
        return 0;
    }
    return buffer;
}
TFR_BYTE TiffDemReader::ReadElevation(int x, int y)
{
    if(GDALRasterIO(hBand, GF_Read, x, y, 1, 1, buffer, 1, 1, GDT_Float32, 0, 0) != CE_None)
    {
        return 0.0;
    }
    //    std::cout << buffer << std::endl;
    return *buffer;
}

TFR_BYTE TiffDemReader::ReadElevationCoord(float lon, float lat)
{
    int x, y;
    ToXY(lon, lat, x, y);
    return ReadElevation(x, y);
}


bool TiffDemReader::Open(const char *tifFileName)
{
    valid = false;
    // 注册所有功能
    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");//以防中文名不能正常读取
    // 打开文件
    hDS = GDALOpen(tifFileName, GA_ReadOnly);
    if(hDS == nullptr)
    {
        fprintf(stderr, "Can't open %s\n", tifFileName);
        return 0.0;
    }
    int iBand = 1;
    hBand = GDALGetRasterBand(hDS, iBand);
    if(hBand == nullptr)
    {
        fprintf(stderr, "Can't get band\n");
        return 0.0;
    }

    //获取坐标变换系数
    if(GDALGetGeoTransform(hDS, m_trans) != CE_None)
    {
        fprintf(stderr, "cant not get geotransform");
        return 0.0;
    }

    //获取图像的尺寸
    nImgSizeX = GDALGetRasterXSize(hDS);
    nImgSizeY = GDALGetRasterYSize(hDS);
    buffer = (TFR_BYTE *)CPLMalloc(sizeof(TFR_BYTE) * 1);
    valid = true;
    return true;
}

void TiffDemReader::Close()
{
    if(buffer)
    {
        CPLFree(buffer);
    }
    buffer = nullptr;

    if(hDS)
    {
        GDALClose(hDS);
    }
    hDS = nullptr;
}

void TiffDemReader::ToCoord(int x, int y, float &lon, float &lat) const
{
    //    if(x < 0 || y < 0 || x >= nImgSizeX || y >= nImgSizeY)return false;
    lon = m_trans[0] + x * m_trans[1] + y * m_trans[2];
    lat = m_trans[3] + x * m_trans[4] + y * m_trans[5];
}
void TiffDemReader::ToCoordDb(double x, double y, float &lon, float &lat)const
{
    //    if(x < 0 || y < 0 || x >= nImgSizeX || y >= nImgSizeY)return false;
    lon = m_trans[0] + x * m_trans[1] + y * m_trans[2];
    lat = m_trans[3] + x * m_trans[4] + y * m_trans[5];
}
void TiffDemReader::ToXY(float lon, float lat, int &x, int &y)const
{
    double dTemp = m_trans[2] * m_trans[4] - m_trans[1] * m_trans[5];
    // 行列号
    double dRow = (m_trans[4] * (lon - m_trans[0]) - m_trans[1] * (lat - m_trans[3])) / dTemp;
    double dCol = (m_trans[2] * (lat - m_trans[3]) - m_trans[5] * (lon - m_trans[0])) / dTemp;
    x = (int)dCol;
    y = (int)dRow;
}

void TiffDemReader::ToXYDb(float lon, float lat, double &x, double &y)const
{
    double dTemp = m_trans[2] * m_trans[4] - m_trans[1] * m_trans[5];
    // 行列号
    double dRow = (m_trans[4] * (lon - m_trans[0]) - m_trans[1] * (lat - m_trans[3])) / dTemp;
    double dCol = (m_trans[2] * (lat - m_trans[3]) - m_trans[5] * (lon - m_trans[0])) / dTemp;
    x = dCol;
    y = dRow;
}

int TiffDemReader::getNImgSizeX() const
{
    return nImgSizeX;
}

int TiffDemReader::getNImgSizeY() const
{
    return nImgSizeY;
}

double TiffDemReader::getStepX() const
{
    return m_trans[1];
}

double TiffDemReader::getStepY() const
{
    return m_trans[5];
}


float TiffDemReader::ReadElevation(const char *tifFileName, float lon, float lat)
{
    // 注册所有功能
    GDALAllRegister();

    // 打开文件
    GDALDatasetH hDS = GDALOpen(tifFileName, GA_ReadOnly);
    if(hDS == nullptr)
    {
        fprintf(stderr, "Can't open %s\n", tifFileName);
        return 0.0;
    }
    int iBand = 1;
    GDALRasterBandH hBand = GDALGetRasterBand(hDS, iBand);
    if(hBand == nullptr)
    {
        fprintf(stderr, "Can't get band\n");
        return 0.0;
    }

    //获取坐标变换系数
    double trans[6] = { 0 };
    if(GDALGetGeoTransform(hDS, trans) != CE_None)
    {
        fprintf(stderr, "cant not get geotransform");
        return 0.0;
    }

    // 计算像素位置
    double dTemp = trans[2] * trans[4] - trans[1] * trans[5];
    // 行列号
    double dRow = (trans[4] * (lon - trans[0]) - trans[1] * (lat - trans[3])) / dTemp;
    double dCol = (trans[2] * (lat - trans[3]) - trans[5] * (lon - trans[0])) / dTemp;
    int dx = (int)dCol;
    int dy = (int)dRow;

    // 获取数据类型
    GDALDataType dType = GDALGetRasterDataType(hBand);
    int dTypeSize = GDALGetDataTypeSizeBytes(dType);
    // 一般高程数据类型就是float，因此不再进行判断，如果为了严谨应该判断类型
    float *buffer = (float *)CPLMalloc(sizeof(float) * 1);
    if(GDALRasterIO(hBand, GF_Read, dx, dy, 1, 1, buffer, 1, 1, GDT_Float32, 0, 0) != CE_None)
    {
        return 0.0;
    }
    float dem = *buffer;
    CPLFree(buffer);
    GDALClose(hDS);
    return dem;
}

void DemContour::getContourKeys(std::vector<DemContour *> &contours, QList<double> &ret)
{
    for(size_t i = 0; i < contours.size(); i++)
    {
        auto dem = contours.at(i)->dem;
        auto canAppend = ret.indexOf(dem) == -1;
        if(canAppend)
        {
            ret.append(dem);
        }
    }
    std::sort(ret.begin(), ret.end(), [](double a, double b)
    {
        return a > b;
    });
}
