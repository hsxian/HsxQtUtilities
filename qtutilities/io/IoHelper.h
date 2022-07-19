#ifndef IOHELPER_H
#define IOHELPER_H

#include <QString>
#include <QStringList>

#include "qtutilities_global.h"

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT IoHelper
    {
    public:
        static IoHelper *instance();
        QStringList findFiles(const QString &dir, QStringList filters);
        void mkDirWhenNotExist(const QString &dir);
        bool readAllFrom(const QString &path, QByteArray &ret);
        bool writeAllTo(const QString &path, QByteArray &text);
    private :
        IoHelper();
        static IoHelper *_instance;
    };
}
#endif // IOHELPER_H
