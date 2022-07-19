#include "ThreadHelper.h"
#include <QEventLoop>
#include <QTimer>
#include <qdatetime.h>

using namespace hsx;

ThreadHelper *ThreadHelper::_instance = 0;
ThreadHelper *ThreadHelper::instance()
{
    if(!_instance)
    {
        _instance = new ThreadHelper();
    }
    return _instance;
}

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

ThreadHelper::ThreadHelper()
{
}
