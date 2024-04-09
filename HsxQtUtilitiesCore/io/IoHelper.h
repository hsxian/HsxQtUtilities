#ifndef IOHELPER_H
#define IOHELPER_H

#include <QString>
#include <QStringList>

#include <HsxQtUtilitiesCore_global.h>

namespace hsx
{
class HSXQTUTILITIESCORE_EXPORT IoHelper
{
public:
    QStringList findFiles(const QString &dir, QStringList filters);
    void mkDirWhenNotExist(const QString &dir);
    bool readAllFrom(const QString &path, QByteArray &ret);
    bool writeAllTo(const QString &path, QByteArray &text);
};
}
#endif // IOHELPER_H
