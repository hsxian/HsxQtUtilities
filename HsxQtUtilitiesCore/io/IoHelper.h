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
    bool isIpLive(const QString &ip, int port, int timeout = 1000);
    bool readAllFrom(const QString &fileName, QByteArray &ret);
    bool writeAllTo(const QString &fileName, const QByteArray &text);
    QString getIpv4();
    void mkDirWhenNotExist(const QString dir);
    void mkDirWhenNotExistWithFile(const QString &file);
    static QChar InvalidFileNameChars[];// = {34, 60, 62, 124, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    QString getValidFileName(const QString fileName, QChar replaceChar = ' ');
    bool removeFile(const QString &file);
    QString guaranteeFullPath(const QString &file);
};
}
#endif // IOHELPER_H
