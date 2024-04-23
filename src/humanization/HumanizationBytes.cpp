#include "HumanizationBytes.h"

hsx::HumanizationBytes::HumanizationBytes()
{

}

QString hsx::HumanizationBytes::toString(size_t size, int precision)
{
    if(size < (1 << 10))
    {
        return QString("%1Bit").arg(size);
    }
    if(size < (1 << 20))
    {
        return QString("%1Kib").arg(QString::number(size / 1024.0, 'f', precision));
    }
    if(size < (1 << 30))
    {
        return QString("%1Mb").arg(QString::number(size / 1024.0 / 1024.0, 'f', precision));
    }

    return QString("%1Gb").arg(QString::number(size / 1024.0 / 1024.0 / 1024.0, 'f', precision));
}
