#include "FileCache.h"
#include <io/IoHelper.h>
#include <QDir>

using namespace hsx;
FileCache::FileCache()
{

}

void FileCache::setCacheDir(const QString &dir)
{
    m_Dir = dir;
    IoHelper::instance()->mkDirWhenNotExist(dir);
}

void FileCache::cleanOutTimeFiles(const std::function<QDateTime(const QFileInfo &)> &checkoutFunc, const QDateTime &currentTime, uint timeoutSecs)
{
    auto fs = IoHelper::instance()->findFiles(m_Dir, QStringList());
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

void FileCache::get(const QString &filename,
                    const std::function<void (QByteArray &)> &pullProxywhenNotFileCache,
                    std::function<bool (QByteArray &)> &canCache,
                    QByteArray &ret)
{
    QDir qdir(m_Dir);
    auto fn = qdir.absoluteFilePath(filename);
    if(IoHelper::instance()->readAllFrom(fn, ret))
    {
        return;
    }
    pullProxywhenNotFileCache(ret);
    if(canCache(ret))
    {
        IoHelper::instance()->writeAllTo(fn, ret);
    }
}
