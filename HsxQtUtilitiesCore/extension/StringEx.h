#ifndef STRINGEX_H
#define STRINGEX_H

#include <QString>

#include <HsxQtUtilitiesCore_global.h>

namespace hsx
{
    class HSXQTUTILITIESCORE_EXPORT StringEx
    {
    public:
        StringEx();
        QString trimTimeStringStart(const QString &str);
    };
}
#endif // STRINGEX_H
