#include "ThreadHelper.h"
#include <QEventLoop>
#include <QTimer>
#include <qdatetime.h>
#include <thread>

using namespace hsx;
void ThreadHelper::delay(int msce)
{
    QEventLoop loop;
    QTimer::singleShot(msce, &loop, &QEventLoop::quit);
    loop.exec();
}

void ThreadHelper::waitOn(const std::function<bool()> &pFunc, int msecsTimecheck, int msecsTimeout)
{
    auto now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    while(pFunc())
    {
        if(QDateTime::currentDateTime().toMSecsSinceEpoch() - now > msecsTimeout)
        {
            break;
        }
        delay(msecsTimecheck);
    }
}
void ThreadHelper::waitOn4std(const std::function<bool ()> &pFunc, int msecsTimecheck, int msecsTimeout)
{
    auto now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    while(pFunc())
    {
        if(QDateTime::currentDateTime().toMSecsSinceEpoch() - now > msecsTimeout)
        {
            qDebug() << __FUNCTION__ << "wait time out " << msecsTimecheck << msecsTimeout;
            break;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(msecsTimecheck));
    }
}
