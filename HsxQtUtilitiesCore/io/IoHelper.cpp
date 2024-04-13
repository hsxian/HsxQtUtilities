#include "IoHelper.h"

#include <QDir>
#include <QHostInfo>
#include <QString>
#include <QTcpSocket>
#include <QDebug>

using namespace hsx;
QChar IoHelper::InvalidFileNameChars[] =
{
    static_cast<QChar>(34),
    static_cast<QChar>(60),
    static_cast<QChar>(62),
    static_cast<QChar>(124),
    static_cast<QChar>(0),
    static_cast<QChar>(1),
    static_cast<QChar>(2),
    static_cast<QChar>(3),
    static_cast<QChar>(4),
    static_cast<QChar>(5),
    static_cast<QChar>(6),
    static_cast<QChar>(7),
    static_cast<QChar>(8),
    static_cast<QChar>(9),
    static_cast<QChar>(10),
    static_cast<QChar>(11),
    static_cast<QChar>(12),
    static_cast<QChar>(13),
    static_cast<QChar>(14),
    static_cast<QChar>(15),
    static_cast<QChar>(16),
    static_cast<QChar>(17),
    static_cast<QChar>(18),
    static_cast<QChar>(19),
    static_cast<QChar>(20),
    static_cast<QChar>(21),
    static_cast<QChar>(22),
    static_cast<QChar>(23),
    static_cast<QChar>(24),
    static_cast<QChar>(25),
    static_cast<QChar>(47)

};


QStringList IoHelper::findFiles(const QString &dir, QStringList filters)
{
    QStringList files ;
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

bool IoHelper::isIpLive(const QString &ip, int port, int timeout)
{
    QTcpSocket tcp;
    tcp.abort();
    tcp.connectToHost(ip, port);

    return tcp.waitForConnected(timeout);
}

bool IoHelper::readAllFrom(const QString &fileName, QByteArray &ret)
{
    QFile file(fileName);
    if(!file.exists())
    {
        return false;
    }
    if(!file.open(QFile::OpenModeFlag::ReadOnly))
    {
        return false;
    }

    auto text = file.readAll();
    ret.swap(text);
    file.close();
    return true;
}

bool IoHelper::writeAllTo(const QString &fileName, const QByteArray &text)
{
    QFile file(fileName);
    if(!file.open(QFile::OpenModeFlag::WriteOnly))
    {
        return false;
    }
    file.write(text);
    file.close();
    return true;
}

QString IoHelper::getIpv4()
{
    auto info = QHostInfo::fromName(QHostInfo::localHostName());
    auto ads = info.addresses();
    foreach(auto ad, ads)
    {
        if(ad.protocol() == QAbstractSocket::IPv4Protocol)
        {
            return ad.toString();
        }
    }
    return "";
}
void IoHelper::mkDirWhenNotExist(const QString dir)
{
    QDir qdir(dir);
    if(!qdir.exists())
    {
        QDir qdir2;
        if(!qdir2.mkpath(dir))
        {
            qDebug() << "dir :" << dir << " mk failed.";
        }
    }
}
QString IoHelper::getValidFileName(const QString fileName, QChar replaceChar)
{
    auto ret = fileName;
    auto size = sizeof(InvalidFileNameChars) / sizeof(InvalidFileNameChars[0]);
    for(int i = 0; i < size; ++i)
    {
        ret = ret.replace(InvalidFileNameChars[i], replaceChar);
        //        qDebug() << ret << QChar(InvalidFileNameChars[i]);
    }

    return ret;
}

bool IoHelper::removeFile(const QString &file)
{
    QFile f(file);
    if(f.exists())
    {
        return f.remove();
    }
    return true;
}

QString IoHelper::guaranteeFullPath(const QString &file)
{
    QFileInfo f(file);
    return f.absoluteFilePath();
}
void IoHelper::mkDirWhenNotExistWithFile(const QString &file)
{
    QFileInfo fi(file);
    mkDirWhenNotExist(fi.dir().path());
}
