#ifndef FILECACHE_H
#define FILECACHE_H

#include <QString>
#include <functional>
#include <QFileInfo>
#include <QDateTime>
#include <QByteArray>

#include <qtutilities_global.h>

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT FileCache
    {
    public:
        FileCache();
        void setCacheDir(const QString &dir);
        void cleanOutTimeFiles(const std::function<QDateTime(const QFileInfo &)> &checkoutFunc, const QDateTime &currentTime, uint timeoutSecs);
        void get(const QString &filename, const std::function<void(QByteArray &)> &pullProxywhenNotFileCache, std::function<bool(QByteArray &)> &canCache, QByteArray &ret);
    private:
        QString m_Dir;
    };
}
#endif // FILECACHE_H
