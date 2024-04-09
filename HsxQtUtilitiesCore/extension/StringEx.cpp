#include "StringEx.h"
#include <QDebug>

hsx::StringEx::StringEx()
{

}

QString hsx::StringEx::trimTimeStringStart(const QString &str)
{
    auto temp = str;
    while(temp.startsWith("0") && temp.size() > 2)
    {
        temp = temp.mid(2);
    }
    return temp;
}
