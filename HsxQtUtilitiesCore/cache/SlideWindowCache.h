#ifndef SLIDEWINDOWCACHE_H
#define SLIDEWINDOWCACHE_H

#include <QString>
#include <QUuid>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QList>
#include <QMap>
#include <thread/ThreadHelper.h>

namespace hsx
{
enum StatusFlags
{
    Running,
    Finished
};

template<class TV>
class ValueTaskModel
{
public:
    QList<TV *>values;
    StatusFlags status = StatusFlags::Running;
    ~ValueTaskModel();
};

template<class TV>
ValueTaskModel<TV>::~ValueTaskModel()
{
    qDeleteAll(values);
    values.clear();
}
template <class TK, class TV>
class SlideWindowConfig
{
public:
    TK step;
    TK start;
    TK end;
    TK cacheSpanLeft;
    TK cacheSpanRight;
    TK autoLoadSpanLeft;
    TK autoLoadSpanRight;
    float getAutoLoadValueSensitivity();
    void setAutoLoadValueSensitivity(float v);
    QString verify();
    std::function<void(const TK &start, const TK &end, QList<TV *> &ret)>valueGetProxyFunc = nullptr;
    std::function<void(const StatusFlags status)>onAutoLoadValuesStatusChangedFunc = nullptr;
private:
    float m_AutoLoadValueSensitivity = 0.5;
};

template<class TK, class TV>
float SlideWindowConfig<TK, TV>::getAutoLoadValueSensitivity()
{
    return m_AutoLoadValueSensitivity;
}

template<class TK, class TV>
void SlideWindowConfig<TK, TV>::setAutoLoadValueSensitivity(float v)
{
    if(v > 0 && v < 1)
    {
        m_AutoLoadValueSensitivity = v;
    }
}

template<class TK, class TV>
QString SlideWindowConfig<TK, TV>::verify()
{
    if(start >= end)
    {
        return "end must greaster than start.";
    }

    auto span = end - start;

    if(step >= span)
    {
        return "the difference between end and start must greaster than step.";
    }

    if(cacheSpanLeft < autoLoadSpanLeft)
    {
        return "cacheSpanLeft must greaster than autoLoadSpanLeft.";
    }

    if(cacheSpanRight < autoLoadSpanRight)
    {
        return "cacheSpanRight must greaster than autoLoadSpanRight.";
    }

    auto  sum = cacheSpanLeft + cacheSpanRight;

    if(sum > span)
    {
        return "the difference between end and start must greaster than the sum between cacheSpanLeft and cacheSpanRight.";
    }

    if(valueGetProxyFunc == nullptr)
    {
        return "must set valueGetProxyFunc";
    }

    return "";
}

template <class TK, class TV>
class SlideWindowCache
{
public:
    SlideWindowCache(SlideWindowConfig<TK, TV> *config);
    ~SlideWindowCache();
    void tryGetValues(const TK &start, const TK &end, QList<TV *> &ret);
    void toUniformKey(const TK &k, TK &ret);
    void getUniformKeys(const TK &start, const TK &end, QList<TK> &ret);
    void setCurrentKeyThenTryAutoLoad(const TK &k);
    void stopAutoLoad();
    TK getCurrentKey();
    uint size();
private:
    QMap<TK, ValueTaskModel<TV>*> *m_Cache = nullptr;
    SlideWindowConfig<TK, TV> *m_Config = nullptr;
    TK m_CurrentAutoLoadKey;
    QList<QUuid> *m_CanRunningUuids = nullptr;
    QList<QUuid> *m_ValueOperationUuids = nullptr;
    void loadValueFromProxy(const TK &start, const TK &end);
    void autoLoadValues(QUuid uid);
    void cleanOutValues(QUuid uid);
};

template<class TK, class TV>
SlideWindowCache<TK, TV>::SlideWindowCache(SlideWindowConfig<TK, TV> *config)
{
    m_Config = config;
    m_Cache = new QMap<TK, ValueTaskModel<TV>*>();
    m_CanRunningUuids = new QList<QUuid>();
    m_ValueOperationUuids = new QList<QUuid>();
}

template<class TK, class TV>
SlideWindowCache<TK, TV>::~SlideWindowCache()
{
    qDebug() << "enter release SlideWindowCache";
    stopAutoLoad();

    while (m_Cache->size())
    {
        auto ks = m_Cache->keys();

        foreach(auto k, ks)
        {
            ValueTaskModel<TV> *v = m_Cache->value(k);

            if(v && v->status == StatusFlags::Finished)
            {
                m_Cache->remove(k);
                delete v;
            }
        }

        qDebug() << "remove cache value, left size:" << m_Cache->size();
    }

    delete m_Cache;
    m_CanRunningUuids->clear();
    delete m_CanRunningUuids;
    m_ValueOperationUuids->clear();
    delete m_ValueOperationUuids;
    qDebug() << "exit release SlideWindowCache";
}

template<class TK, class TV>
void SlideWindowCache<TK, TV>::tryGetValues(const TK &start, const TK &end, QList<TV *> &ret)
{
    QList<TK>ks;
    getUniformKeys(start, end, ks);

    foreach(auto k, ks)
    {
        if(m_Cache->contains(k))
        {
            ValueTaskModel<TV> *v = m_Cache->value(k);

            if(v && v->status == StatusFlags::Finished)
            {
                ret.append(v->values);
            }
        }
        else
        {
            loadValueFromProxy(k, k + m_Config->step);
        }
    }
}

template<class TK, class TV>
void SlideWindowCache<TK, TV>::toUniformKey(const TK &k, TK &ret)
{
    auto c = (TK)(k / m_Config->step);
    ret = c * m_Config->step;
}

template<class TK, class TV>
void SlideWindowCache<TK, TV>::getUniformKeys(const TK &start, const TK &end, QList<TK> &ret)
{
    TK us, ue;
    toUniformKey(start, us);
    toUniformKey(end, ue);
    auto s = us > m_Config->start ? us - m_Config->step : us;
    auto e = ue < m_Config->end ? ue + m_Config->step : ue;

    for(TK k = s; k <= e; k += m_Config->step)
    {
        ret.append(k);
    }
}

template<class TK, class TV>
void SlideWindowCache<TK, TV>::setCurrentKeyThenTryAutoLoad(const TK &k)
{
    auto needRestart = false;

    if(k > m_CurrentAutoLoadKey)
    {
        auto diff = k - m_CurrentAutoLoadKey;

        if(diff > m_Config->getAutoLoadValueSensitivity()*m_Config->autoLoadSpanRight)
        {
            needRestart = true;
            qDebug() << QString("set current key when diff > %1 by right").arg(m_Config->getAutoLoadValueSensitivity());
        }
    }
    else
    {
        auto diff = m_CurrentAutoLoadKey - k;

        if(diff > m_Config->getAutoLoadValueSensitivity()*m_Config->autoLoadSpanLeft)
        {
            needRestart = true;
            qDebug() << QString("set current key when diff > %1 by left").arg(m_Config->getAutoLoadValueSensitivity());
        }
    }

    if(needRestart)
    {
        m_CanRunningUuids->clear();
        m_CurrentAutoLoadKey = k;
        QtConcurrent::run([ = ]()
        {
            qDebug() << "enter auto load values thread" << QThread::currentThreadId();

            if(m_Config->onAutoLoadValuesStatusChangedFunc)
            {
                m_Config->onAutoLoadValuesStatusChangedFunc(StatusFlags::Running);
            }

            auto uid = QUuid::createUuid();
            m_CanRunningUuids->append(uid);
            autoLoadValues(uid);
            cleanOutValues(uid);
            m_CanRunningUuids->removeOne(uid);

            if(m_Config->onAutoLoadValuesStatusChangedFunc)
            {
                m_Config->onAutoLoadValuesStatusChangedFunc(StatusFlags::Finished);
            }

            qDebug() << "exit auto load values thread" << QThread::currentThreadId();
        });
    }
}

template<class TK, class TV>
void SlideWindowCache<TK, TV>::stopAutoLoad()
{
    ThreadHelper::instance()->waitOn([ = ]()
    {
        m_CanRunningUuids->clear();
        return m_ValueOperationUuids->size();
    }, 100, 600000);
}

template<class TK, class TV>
TK SlideWindowCache<TK, TV>::getCurrentKey()
{
    return m_CurrentAutoLoadKey;
}

template<class TK, class TV>
uint SlideWindowCache<TK, TV>::size()
{
    return m_Cache->size();
}

template<class TK, class TV>
void SlideWindowCache<TK, TV>::loadValueFromProxy(const TK &start, const TK &end)
{
    auto uid = QUuid::createUuid();
    m_ValueOperationUuids->append(uid);

    if(!m_Cache->contains(start))
    {
        auto v = new ValueTaskModel<TV>();
        m_Cache->insert(start, v);
        m_Config->valueGetProxyFunc(start, end, v->values);
        v->status = StatusFlags::Finished;
    }

    m_ValueOperationUuids->removeOne(uid);
}

template<class TK, class TV>
void SlideWindowCache<TK, TV>::autoLoadValues(QUuid uid)
{
    auto s = m_CurrentAutoLoadKey - m_Config->autoLoadSpanLeft;
    auto e = m_CurrentAutoLoadKey + m_Config->autoLoadSpanRight;
    QList<TK>ks;
    getUniformKeys(m_CurrentAutoLoadKey, e, ks);
    auto c = ks.size() - 1;

    for(int i = 0; i < c; ++i)
    {
        auto k = ks.at(i);
        loadValueFromProxy(k, k + m_Config->step);

        if(!m_CanRunningUuids->contains(uid))
        {
            qDebug() << "cancel auto load on right" << uid;
            return;
        }
    }

    ks.clear();
    getUniformKeys(s, m_CurrentAutoLoadKey,  ks);

    for(int i = ks.size() - 1; i > 0; --i)
    {
        auto k = ks.at(i);
        loadValueFromProxy(k - m_Config->step, k);

        if(!m_CanRunningUuids->contains(uid))
        {
            qDebug() << "cancel auto load on left" << uid;
            return;
        }
    }
}

template<class TK, class TV>
void SlideWindowCache<TK, TV>::cleanOutValues(QUuid uid)
{
    qDebug() << "enter clean out values";
    m_ValueOperationUuids->append(uid);
    auto c = 0;
    auto s = m_CurrentAutoLoadKey - m_Config->cacheSpanLeft - m_Config->step;
    auto e = m_CurrentAutoLoadKey + m_Config->cacheSpanRight + m_Config->step;
    auto ks = m_Cache->keys();

    foreach(auto k, ks)
    {
        if(k < s || s > e)
        {
            ValueTaskModel<TV> *v = m_Cache->value(k);

            if(v && v->status == StatusFlags::Finished)
            {
                m_Cache->remove(k);
                delete v;
                c++;
            }
        }
    }

    m_ValueOperationUuids->removeOne(uid);
    qDebug() << "exit clean out values, remove size:" << c;
}
}
#endif // SLIDEWINDOWCACHE_H
