#ifndef TIFFBOX_H
#define TIFFBOX_H

#include "HsxQtUtilitiesGis_global.h"
namespace hsx
{
class TiffDemReader;
/**
 * @brief tiff 区块
 * @author hsx
 */
class HSXQTUTILITIESGIS_EXPORT TiffBox
{
public:
    TiffBox();
    int xMin, yMin, xMax, yMax, xSize, ySize;
    float lonMin, latMin, lonMax, latMax;
    int xMinEx, yMinEx, xMaxEx, yMaxEx, xSizeEx, ySizeEx;
    float lonMinEx, latMinEx, lonMaxEx, latMaxEx;
    /**
     * @brief 从行列初始化
     * @param tdr
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param unifyWidthAndHeight 调整为正方形
     */
    void InitFromXY(const TiffDemReader &tdr, int x1, int y1, int x2, int y2, bool unifyWidthAndHeight);
    /**
     * @brief 扩大倍数
     * @param tdr
     * @param s
     */
    void Enlarge(const TiffDemReader &tdr, float s);
    bool m_unifyWidthAndHeight = false;
};
}
#endif // TIFFBOX_H
