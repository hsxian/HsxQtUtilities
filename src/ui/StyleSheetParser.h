#ifndef STYLESHEETPARSER_H
#define STYLESHEETPARSER_H

#include <HsxQtUtilities_global.h>

namespace hsx
{
class HSXQTUTILITIES_EXPORT StyleSheetParser
{
public:
    StyleSheetParser();
    void parse(const QString &qss);
};
}
#endif // STYLESHEETPARSER_H
