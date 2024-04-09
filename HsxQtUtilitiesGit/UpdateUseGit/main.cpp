#include "git/GitHelper.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <humanization/HumanizationBytes.h>
using namespace hsx;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GitHelper gh;
    //    gh.open("../../../libgit2");
    QDir dir("hsx");
    if(dir.exists())
    {
        auto rmres = dir.removeRecursively();
        if(!rmres)
        {
            qDebug() << "remove dir " << dir.absolutePath() << " failed.";
            return 0;
        }
    }
    gh.sidebandProgressFunc = [](const QString & str)
    {
        qDebug() << str;
    };
    gh.transferProgressFunc = [](const GitTransferProgress & stats)
    {

        qDebug() << stats.indexed_deltas
                 << stats.total_deltas
                 << stats.indexed_objects
                 << stats.local_objects
                 << stats.received_objects
                 << stats.total_objects
                 << stats.received_bytes
                 << hsx::HumanizationBytes::toString(stats.received_bytes)
                 << QDateTime::currentDateTime().time()
                 ;
    };
    gh.clone("https://gitee.com/cenix/gitest.git", "hsx");
    gh.open("hsx");
    gh.checkout(GitTypes::GIT_CHECKOUT_FORCE, QStringList());
    gh.test();

    return a.exec();
}
