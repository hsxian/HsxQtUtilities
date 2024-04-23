#ifndef HUMANIZATIONBYTES_H
#define HUMANIZATIONBYTES_H

#include <QString>
#include <HsxQtUtilities_global.h>
namespace hsx
{
    class HSXQTUTILITIES_EXPORT HumanizationBytes
    {
    public:
        HumanizationBytes();
        static QString toString(size_t size, int precision = 2);
    };

}

#endif // HUMANIZATIONBYTES_H
