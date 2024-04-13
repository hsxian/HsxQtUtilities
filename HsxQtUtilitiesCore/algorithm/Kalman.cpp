#include "Kalman.h"

#include <QPointF>
namespace hsx
{
Kalman::Kalman()
{

}

void Kalman::predict(QList<double> &ret, QList<double> &rawDate, double q, double r)
{
    auto pCount = rawDate.size();
    if(pCount < 1)
    {
        return;
    }

    double xLast = 0;
    double pLast = 0.02;
    double kg;
    double xMid;
    double xNow;
    double pMid;
    double pNow;
    //double z_real = 0.56;
    double zMeasure;
    //double sumerror_kalman = 0;
    //double sumerror_measure = 0;

    xLast = rawDate[0];
    xMid = xLast;

    for(auto i = 0; i < pCount; i++)
    {
        xMid = xLast;
        pMid = pLast + q;
        kg = pMid / (pMid + r);
        zMeasure = rawDate[i];
        xNow = xMid + kg * (zMeasure - xMid);
        ret.append(xNow);
        pNow = (1 - kg) * pMid;
        //sumerror_kalman += Math.Abs(z_real - x_now);
        //sumerror_measure += Math.Abs(z_real - z_measure);
        pLast = pNow;
        xLast = xNow;
    }
}

void Kalman::predictY(QList<QPointF> &inOut, double q, double r)
{
    auto pCount = inOut.size();
    if(pCount < 1)
    {
        return;
    }

    double xLast = 0;
    double pLast = 0.02;
    double kg;
    double xMid;
    double xNow;
    double pMid;
    double pNow;
    //double z_real = 0.56;
    double zMeasure;
    //double sumerror_kalman = 0;
    //double sumerror_measure = 0;

    xLast = inOut[0].y();
    xMid = xLast;

    for(auto i = 0; i < pCount; i++)
    {
        xMid = xLast;
        pMid = pLast + q;
        kg = pMid / (pMid + r);
        zMeasure = inOut[i].y();
        xNow = xMid + kg * (zMeasure - xMid);
        inOut[i].setY(xNow);
        pNow = (1 - kg) * pMid;
        //sumerror_kalman += Math.Abs(z_real - x_now);
        //sumerror_measure += Math.Abs(z_real - z_measure);
        pLast = pNow;
        xLast = xNow;
    }
}
}
