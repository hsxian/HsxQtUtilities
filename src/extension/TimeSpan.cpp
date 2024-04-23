#include "TimeSpan.h"


hsx::TimeSpan::TimeSpan(unsigned long ms)
{
    unsigned int secInt = 1000;
    unsigned int minInt = secInt * 60;
    unsigned int houInt = minInt * 60;
    unsigned int dayInt = houInt * 24;

    day = ms / dayInt;
    hour = (ms - day * dayInt) / houInt;
    minute = (ms - day * dayInt - hour * houInt) / minInt;
    second = (ms - day * dayInt - hour * houInt - minute * minInt) / secInt;
    milliSecond = ms - day * dayInt - hour * houInt - minute * minInt - second * secInt;
}

QString hsx::TimeSpan::toMsString(const QString &d, const QString &h, const QString &mi, const QString &s, const QString &ms)
{
    return QString("%1%2%3%4%5%6%7%8%9%10")
           .arg(day)
           .arg(d)
           .arg(hour)
           .arg(h)
           .arg(minute)
           .arg(mi)
           .arg(second)
           .arg(s)
           .arg(milliSecond)
           .arg(ms);
}

QString hsx::TimeSpan::toScString(const QString &d, const QString &h, const QString &mi, const QString &s)
{
    return QString("%1%2%3%4%5%6%7%8")
           .arg(day)
           .arg(d)
           .arg(hour)
           .arg(h)
           .arg(minute)
           .arg(mi)
           .arg(second)
           .arg(s);
}
