#include <QTest>

#include "OpencvHelperTest.cpp"
#include "SlideWinowCacheTest.h"
#include <QCoreApplication>

int test(QObjectList &lt, int argc, char **argv)
{
    auto ret = 0;
    foreach(auto l, lt)
    {

        ret += QTest::qExec(l, argc, argv);
    }
    return ret;
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QObjectList lt;

    SlideWinowCacheTest swct;
    OpencvHelperTest  oht;
    lt << &oht
       << &swct
       ;
    auto count = lt.size();
    auto ret = test(lt, argc, argv);
    qDebug() << "------"
             << count << "total,"
             << (count - ret) << "passed,"
             << ret << "failed"
             << "------";

}
