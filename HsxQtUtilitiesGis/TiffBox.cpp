#include "TiffBox.h"
#include <algorithm>
#include "TiffDemReader.h"

using namespace std;
using namespace hsx;
TiffBox::TiffBox()
{

}

void TiffBox::InitFromXY(const TiffDemReader &tdr, int x1, int y1, int x2, int y2, bool unifyWidthAndHeight)
{
    xMinEx = xMin = min(x1, x2);
    yMinEx = yMin = min(y1, y2);
    xMaxEx = xMax = max(x1, x2);
    yMaxEx = yMax = max(y1, y2);
    xSizeEx =  xSize = xMax - xMin;
    ySizeEx =  ySize = yMax - yMin;
    m_unifyWidthAndHeight = unifyWidthAndHeight;
    if(m_unifyWidthAndHeight)
    {
        xSizeEx = ySizeEx = xSize = ySize = max(xSize, ySize);
        auto xCenter = (xMin + xMax) / 2;
        auto yCenter = (yMin + yMax) / 2;
        auto halfSize = ySizeEx / 2;
        xMinEx = xMin = xCenter - halfSize;
        yMinEx = yMin = yCenter - halfSize;
        xMaxEx = xMax = xCenter + halfSize;
        yMaxEx = yMax = yCenter + halfSize;
    }
    tdr.ToCoord(xMinEx, yMinEx, lonMin, latMin);
    tdr.ToCoord(xMaxEx, yMaxEx, lonMax, latMax);
    lonMinEx = lonMin;
    latMinEx = latMin;
    lonMaxEx = lonMax;
    latMaxEx = latMax;
}

void TiffBox::Enlarge(const TiffDemReader &tdr, float s)
{
    if(s < 1)
    {
        return;
    }
    auto edgeScale = (s - 1) / 2;
    auto xEdgeSize = xSize * edgeScale;
    auto yEdgeSize = ySize * edgeScale;
    xMinEx -= xEdgeSize;
    yMinEx -= yEdgeSize;
    xMaxEx += xEdgeSize;
    yMaxEx += yEdgeSize;
    xSizeEx = xMaxEx - xMinEx;
    ySizeEx = yMaxEx - yMinEx;
    tdr.ToCoord(xMinEx, yMinEx, lonMinEx, latMinEx);
    tdr.ToCoord(xMaxEx, yMaxEx, lonMaxEx, latMaxEx);
}

