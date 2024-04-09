#ifndef HUMANIZATIONBYTES_H
#define HUMANIZATIONBYTES_H

#include <QString>
#include <HsxQtUtilitiesCore_global.h>
namespace hsx
{
    class HSXQTUTILITIESCORE_EXPORT HumanizationBytes
    {
    public:
        HumanizationBytes();
        static QString toString(size_t size, int precision = 2);
    };

}

#endif // HUMANIZATIONBYTES_H
