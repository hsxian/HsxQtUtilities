#ifndef KALMAN_H
#define KALMAN_H

#include <QList>
#include <HsxQtUtilities_global.h>.h>
class QPointF;
namespace hsx
{
class HSXQTUTILITIES_EXPORT Kalman
{
public:
    Kalman();
    void predict(QList<double> &ret, QList<double> &rawDate, double q = 0.018, double r = 0.542);
    void predictY(QList<QPointF> &inOut, double q = 0.018, double r = 0.542);
};
}
#endif // KALMAN_H
