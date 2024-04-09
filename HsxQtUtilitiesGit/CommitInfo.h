#ifndef COMMITINFO_H
#define COMMITINFO_H

#include <QDateTime>
#include <QString>
#include <HsxQtUtilitiesGit_global.h>

namespace hsx
{
    class HSXQTUTILITIESGIT_EXPORT  CommitInfo
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
