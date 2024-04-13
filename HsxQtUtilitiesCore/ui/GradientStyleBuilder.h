#ifndef GRADIENTSTYLEBUILDER_H
#define GRADIENTSTYLEBUILDER_H
#include <HsxQtUtilitiesCore_global.h>

#include <QColor>
namespace hsx
{

/**
 * @brief The 渐变样式构造器 class
 * @author hsx
 */
class HSXQTUTILITIESCORE_EXPORT GradientStyleBuilder
{
public:
    /**
     * @brief 初始化线性渐变
     * @param 起点x1
     * @param 起点y1
     * @param 终点x2
     * @param 终点y2
     * @return
     */
    GradientStyleBuilder &initLinear(float x1 = 0, float y1 = 0, float x2 = 1, float y2 = 1);
    /**
     * @brief 初始化圆锥渐变
     * @param 圆心cx
     * @param 圆心cy
     * @param 起始角angle
     * @return
     */
    GradientStyleBuilder &initConical(float cx = 0.5, float cy = 0.5, float angle = 0);
    /**
     * @brief 初始化辐射渐变
     * @param 圆心cx
     * @param 圆心cy
     * @param 半径radius
     * @param 渐变起始点fx
     * @param 渐变起始点fy
     * @return
     */
    GradientStyleBuilder &initRadial(float cx = 0.5, float cy = 0.5, float radius = 1, float fx = 0.5, float fy = 0.5);
    /**
     * @brief 设置pos处的颜色
     * @param pos
     * @param color
     * @return
     */
    GradientStyleBuilder &setColorAt(float pos, const QColor &color);
    QString toQSS(const QString &wrap = "background");

    QString toQString();
private:
    QString m_QSS;
};
}
#endif // GRADIENTSTYLEBUILDER_H
