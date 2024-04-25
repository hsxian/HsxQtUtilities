#include "SlideWinowCacheTest.h"
#include <QTest>
#include <thread/ThreadHelper.h>

SlideWinowCacheTest::SlideWinowCacheTest()
{
    //    app->processEvents();
    m_Config = new hsx::SlideWindowConfig<uint, QString>();
    m_Cache = new  hsx::SlideWindowCache<uint, QString>(m_Config);
}


SlideWinowCacheTest::~SlideWinowCacheTest()
{
    m_Cache->stopAutoLoad();
    delete m_Cache;
    delete m_Config;
}

void SlideWinowCacheTest::test_config()
{
    m_Config->step = 5;
    m_Config->start = 33;
    m_Config->end = 985;
    m_Config->cacheSpanLeft = 100;
    m_Config->cacheSpanRight = 200;
    m_Config->autoLoadSpanLeft = 30;
    m_Config->autoLoadSpanRight = 70;
    m_Config->valueGetProxyFunc = [ = ](const uint & start, const uint & end, QList<QString *> &ret)
    {
        hsx::ThreadHelper th;
        th.delay(10);
        ret.append(new QString(QString::number(start) + "_" + QString::number(end)));
    };
    m_Config->onAutoLoadValuesStatusChangedFunc = [&](hsx::StatusFlags s)
    {
        m_Status = s;
    };
    auto svs = 0.5;
    m_Config->setAutoLoadValueSensitivity(svs);
    QVERIFY(m_Config->getAutoLoadValueSensitivity() == svs);
    m_Config->setAutoLoadValueSensitivity(-1);
    QVERIFY(m_Config->getAutoLoadValueSensitivity() == svs);
    m_Config->setAutoLoadValueSensitivity(2);
    QVERIFY(m_Config->getAutoLoadValueSensitivity() == svs);
    auto error  = m_Config->verify();
    QVERIFY2(error.isEmpty(), error.toStdString().c_str());
}

void SlideWinowCacheTest::test_value_get()
{
    uint s = 689, e = 757;
    QList<QString *>ret;
    m_Cache->tryGetValues(s, e, ret);
    uint count = (e - s) / m_Config->step + 2;
    QVERIFY(count == ret.size());

    ret.clear();
    e = s;
    count = 1;
    m_Cache->tryGetValues(s, e, ret);
    QVERIFY(count == ret.size());

    ret.clear();
    e = s + m_Config->step;
    count = 2;
    m_Cache->tryGetValues(s, e, ret);
    QVERIFY(count == ret.size());

}

void SlideWinowCacheTest::test_uniform_key()
{
    uint uk = 0;
    m_Cache->toUniformKey(78, uk);
    QVERIFY(uk == 75);

    m_Cache->toUniformKey(1, uk);
    QVERIFY(uk == 30);

    m_Cache->toUniformKey(10000, uk);
    QVERIFY(uk == 985);

    QList<uint>uks;
    m_Cache->getUniformKeys(13, 28, uks);
    QVERIFY(uks.size() == 1);

    uks.clear();
    m_Cache->getUniformKeys(113, 128, uks);
    QVERIFY(uks.size() == 4);
}

void SlideWinowCacheTest::test_auto_load()
{
    //first set key to 300
    uint currentKey = 300;
    m_Cache->setCurrentKeyThenTryAutoLoad(currentKey);
    hsx::ThreadHelper th;
    th.waitOn([&]()
    {
        return m_Status == hsx::StatusFlags::Running;
    }, 100, 60000000);
    auto count = (m_Config->autoLoadSpanLeft + m_Config->autoLoadSpanRight) / m_Config->step;
    QVERIFY(count == m_Cache->size());
    QVERIFY(currentKey == m_Cache->getCurrentKey());

    //set key is failed, count and key not changed.
    m_Cache->setCurrentKeyThenTryAutoLoad(currentKey + m_Config->autoLoadSpanRight * m_Config->getAutoLoadValueSensitivity());
    th.waitOn([&]()
    {
        return m_Status == hsx::StatusFlags::Running;
    }, 100, 60000000);
    QVERIFY(count == m_Cache->size());
    QVERIFY(currentKey == m_Cache->getCurrentKey());

    //set key is success.
    m_Status = hsx::StatusFlags::Running;
    currentKey = currentKey + m_Config->autoLoadSpanRight * m_Config->getAutoLoadValueSensitivity() + m_Config->step;
    m_Cache->setCurrentKeyThenTryAutoLoad(currentKey);
    th.waitOn([&]()
    {
        return m_Status == hsx::StatusFlags::Running;
    }, 100, 60000000);
    auto newCount = count + (m_Config->autoLoadSpanRight * m_Config->getAutoLoadValueSensitivity() + m_Config->step) / m_Config->step;
    QVERIFY(newCount == m_Cache->size());
    QVERIFY(currentKey);

    // set key to 700.then could exec auto clean,and auto load value size == first count.
    m_Status = hsx::StatusFlags::Running;
    currentKey = 700;
    m_Cache->setCurrentKeyThenTryAutoLoad(currentKey);
    th.waitOn([ = ]()
    {
        return m_Status == hsx::StatusFlags::Running;
    }, 100, 60000000);
    QVERIFY(count == m_Cache->size());
    QVERIFY(currentKey == m_Cache->getCurrentKey());
}
