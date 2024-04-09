#ifndef FILECACHE_H
#define FILECACHE_H

#include <QString>
#include <functional>
#include <QFileInfo>
#include <QDateTime>
#include <QByteArray>

#include <HsxQtUtilitiesCore_global.h>

namespace hsx
{
class HSXQTUTILITIESCORE_EXPORT FileCache
{
public:
    FileCache();
    void cleanOutTimeFiles(const QString &dir, const std::function<QDateTime(const QFileInfo &)> &checkoutFunc, const QDateTime &currentTime, uint timeoutSecs);
    bool get(const QString &file, const std::function<void(QByteArray &)> &pullProxywhenNotFileCache, const std::function<bool(QByteArray &)> &canCacheFunc, QByteArray &ret);
};
}
#endif // FILECACHE_H
