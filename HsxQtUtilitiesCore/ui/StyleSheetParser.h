#ifndef STYLESHEETPARSER_H
#define STYLESHEETPARSER_H

#include <HsxQtUtilitiesCore_global.h>

namespace hsx
{
class HSXQTUTILITIESCORE_EXPORT StyleSheetParser
{
public:
    StyleSheetParser();
    void parse(const QString &qss);
};
}
#endif // STYLESHEETPARSER_H
