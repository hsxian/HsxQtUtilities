#ifndef COMMITINFO_H
#define COMMITINFO_H

#include <QDateTime>
#include <QString>
#include <HsxQtUtilities_global.h>

namespace hsx
{
/**
* @brief The CommitInfo class
*/
class HSXQTUTILITIES_EXPORT  CommitInfo
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
