#ifndef TIFFDEMREADER_H
#define TIFFDEMREADER_H

#include <gdal.h>
#include <geos/geom/Coordinate.h>
#include "HsxQtUtilitiesGis_global.h"

#define TFR_BYTE float    //方便数据类型的修改
#define GdalByteType GDALDataType::GDT_Float32
namespace hsx
{
/**
 * @brief The DemContour class
 * @author hsx
 */
class HSXQTUTILITIESGIS_EXPORT DemContour
{
public:
    double dem;
    std::vector<geos::geom::Coordinate>points;
    std::vector<geos::geom::Coordinate>pointsOfClose;
    bool hadClose = false;
    bool isValid = true;
    bool isBox = false;
    /**
     * @brief 获取等高线的高度列表（去重）
     * @param contours
     * @param ret
     */
    static void getContourKeys(std::vector<DemContour *> &contours, QList<double> &ret);
};

/**
 * @brief The TiffDemReader class
 * @author hsx
 */
class HSXQTUTILITIESGIS_EXPORT TiffDemReader
{
public:
    TiffDemReader();
    ~TiffDemReader();
    TFR_BYTE ReadElevation(const char *tifFileName, float lon, float lat);
    TFR_BYTE ReadElevation(int x, int y);
    TFR_BYTE ReadElevationCoord(float lon, float lat);
    bool Open(const char *tifFileName);
    void Close();
    /**
     * @brief 行列转经纬度
     * @param x
     * @param y
     * @param lon
     * @param lat
     */
    void ToCoord(int x, int y, float &lon, float &lat) const;
    /**
     * @brief 经纬度转行列
     * @param lon
     * @param lat
     * @param x
     * @param y
     */
    void ToXY(float lon, float lat, int &x, int &y)const;
    void ToXYDb(float lon, float lat, double &x, double &y)const;
    int getNImgSizeX() const;
    int getNImgSizeY() const;
    double getStepX() const;
    double getStepY() const;
    /**
     * @brief 生成等高线
     * @param sLon
     * @param sLat
     * @param eLon
     * @param eLat
     * @param enlargeScale
     * @param dfContourInterval
     * @param dfContourBase
     * @param isCloseContour
     * @param ret
     */
    void ContourGenerate(float sLon, float sLat, float eLon, float eLat, float enlargeScale, double dfContourInterval, double dfContourBase, bool isCloseContour, std::vector<DemContour *> &ret);
    float *ReadElevation(int nDSXOff, int nDSYOff, int nDSXSize, int nDSYSize);
    void ToCoordDb(double x, double y, float &lon, float &lat)const;
    void ContourGenerateXY(int x, int y, int width, int height, double dfContourInterval, double dfContourBase, std::vector<DemContour *> &ret);
    void CloseDemContour(DemContour *contour, double interval);
    bool isValid();
private:
    //坐标变换系数
    double m_trans[6];
    TFR_BYTE *buffer = nullptr;
    GDALDatasetH hDS = nullptr;
    GDALRasterBandH hBand = nullptr;
    int nImgSizeX = 0;
    int nImgSizeY = 0;
    bool valid = false;
};
}
#endif // TIFFDEMREADER_H
