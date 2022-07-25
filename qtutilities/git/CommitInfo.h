#ifndef COMMITINFO_H
#define COMMITINFO_H

#include <QDateTime>
#include <QString>
#include <qtutilities_global.h>

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT  CommitInfo
    {
    public:
        CommitInfo();
        QString oid;
        QString message;
        QString author;
        QDateTime time;
    };
}
#endif // COMMITINFO_H
