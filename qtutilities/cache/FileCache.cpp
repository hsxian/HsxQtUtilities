#include "FileCache.h"
#include <io/IoHelper.h>
#include <QDir>

using namespace hsx;
FileCache::FileCache()
{
}


void FileCache::cleanOutTimeFiles(const QString &dir, const std::function<QDateTime(const QFileInfo &)> &checkoutFunc, const QDateTime &currentTime, uint timeoutSecs)
{
    auto fs = IoHelper::instance()->findFiles(dir, QStringList());
    foreach(auto f, fs)
    {
        QFileInfo fi(f);
        auto t = checkoutFunc(fi);
        auto diff = t.secsTo(currentTime);
        if(diff > timeoutSecs)
        {
            QFile qf(f);
            qf.remove();
        }
    }
}

bool FileCache::get(const QString &file,
                    const std::function<void(QByteArray &)> &pullProxywhenNotFileCache,
                    const std::function<bool(QByteArray &)> &canCacheFunc,
                    QByteArray &ret)
{
    if(IoHelper::instance()->readAllFrom(file, ret))
    {
        return true;
    }
    pullProxywhenNotFileCache(ret);
    if(canCacheFunc(ret))
    {
        IoHelper::instance()->writeAllTo(file, ret);
        return true;
    }
    return false;
}
