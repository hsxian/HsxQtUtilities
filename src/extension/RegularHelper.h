#ifndef REGULARHELPER_H
#define REGULARHELPER_H

#include "HsxQtUtilities_global.h"

#include <QStringList>
namespace hsx {
class HSXQTUTILITIES_EXPORT RegularHelper
{
public:
    QStringList findAll(const QString& pattern, const QString& text);
};
}

#endif // REGULARHELPER_H
