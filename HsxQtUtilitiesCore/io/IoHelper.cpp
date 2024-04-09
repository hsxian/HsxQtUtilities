#include "IoHelper.h"

#include <QDir>
#include <QString>
#include <QDebug>

using namespace hsx;

QStringList IoHelper::findFiles(const QString &dir, QStringList filters)
{
    QStringList files;
    QDir qdir(dir);
    foreach(QString file, qdir.entryList(filters, QDir::Files))
    {
        files += dir + '/' + file;
    }
    foreach(QString subdir, qdir.entryList(filters, QDir::AllDirs | QDir::NoDotAndDotDot))
    {
        files += findFiles(dir + '/' + subdir, filters);
    }
    return files;
}

void IoHelper::mkDirWhenNotExist(const QString &dir)
{
    QDir qdir(dir);
    if(!qdir.exists())
    {
        if(!qdir.mkdir(dir))
        {
            qDebug() << "IoHelper::mkDirWhenNotExist, mk dir failed: " << dir;
        }
    }
}

bool IoHelper::readAllFrom(const QString &path, QByteArray &ret)
{
    QFile f(path);
    if(!f.exists())
    {
        return false;
    }
    if(!f.open(QFile::OpenModeFlag::ReadOnly))
    {
        return false;
    }
    ret = f.readAll();
    f.close();
    return true;
}

bool IoHelper::writeAllTo(const QString &path, QByteArray &text)
{
    QFile f(path);
    if(!f.open(QFile::OpenModeFlag::WriteOnly))
    {
        return false;
    }
    auto res = f.write(text);
    f.close();
    return res;
}
