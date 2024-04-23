#include "RegularHelper.h"

#include <QRegularExpression>

using namespace hsx;

QStringList RegularHelper::findAll(const QString& pattern, const QString& text)
{
    QStringList ret;
    QRegularExpression reg(pattern);
    auto res = reg.globalMatch(text);

    while(res.hasNext())
    {
        auto match = res.next();
        ret << match.captured();
    }
    return ret;
}
