#include "GradientStyleBuilder.h"
#include <QDebug>
using namespace hsx;
GradientStyleBuilder &GradientStyleBuilder::initLinear(float x1, float y1, float x2, float y2)
{
    m_QSS = QString("qlineargradient(x1:%1, y1:%2, x2:%3, y2:%4")
            .arg(x1)
            .arg(y1)
            .arg(x2)
            .arg(y2)
            ;
    return *this;
}

GradientStyleBuilder &GradientStyleBuilder::initConical(float cx, float cy, float angle)
{
    m_QSS = QString("qconicalgradient(cx:%1, cy:%2, angle:%3")
            .arg(cx)
            .arg(cy)
            .arg(angle)
            ;
    return *this;
}

GradientStyleBuilder &GradientStyleBuilder::initRadial(float cx, float cy, float radius, float fx, float fy)
{
    m_QSS = QString("qradialgradient(cx:%1, cy:%2, radius:%3, fx:%4, fy:%5")
            .arg(cx)
            .arg(cy)
            .arg(radius)
            .arg(fx)
            .arg(fy)
            ;

    return *this;
}

GradientStyleBuilder &GradientStyleBuilder::setColorAt(float pos, const QColor &color)
{
    m_QSS += QString(", stop:%1  %2").arg(pos).arg(color.name(QColor::NameFormat::HexArgb));
    return *this;
}
QString GradientStyleBuilder::toQString()
{
    m_QSS += ")";
    return m_QSS;
}
QString GradientStyleBuilder::toQSS(const QString &wrap)
{
    m_QSS = toQString();
    if(!wrap.isEmpty())
    {
        m_QSS = wrap + ":" + m_QSS;
    }
    qDebug() << __FUNCTION__ << m_QSS;
    return m_QSS;
}

