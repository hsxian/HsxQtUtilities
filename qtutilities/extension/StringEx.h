#ifndef STRINGEX_H
#define STRINGEX_H

#include <QString>

#include <qtutilities_global.h>

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT StringEx
    {
    public:
        StringEx();
        QString trimTimeStringStart(const QString &str);
    };
}
#endif // STRINGEX_H
