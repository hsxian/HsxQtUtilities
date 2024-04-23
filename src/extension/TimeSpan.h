#pragma execution_character_set("utf-8")
#ifndef TIMESPAN_H
#define TIMESPAN_H

#include <QString>

#include <HsxQtUtilities_global.h>

namespace hsx
{
    class HSXQTUTILITIES_EXPORT  TimeSpan
    {
    public:
        TimeSpan(unsigned long ms);
        long day = 0;
        long hour = 0;
        long minute = 0;
        long second = 0;
        long milliSecond = 0;
        QString toMsString(const QString &d = "天", const QString &h = "时", const QString &mi = "分", const QString &s = "秒", const QString &ms = "毫秒");
        QString toScString(const QString &d = "天", const QString &h = "时", const QString &mi = "分", const QString &s = "秒");
    };
}
#endif // TIMESPAN_H
