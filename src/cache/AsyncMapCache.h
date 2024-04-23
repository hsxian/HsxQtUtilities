#ifndef ASYNCMAPCACHE_H
#define ASYNCMAPCACHE_H

#include <QMap>
#include <functional>
#include <QDebug>
#include <atomic>
namespace hsx
{
template<class TKey, class TValue>
/**
 * @brief 异步字典缓存
 * @author hsx
 */
class AsyncMapCache
{
public:
    AsyncMapCache() {};
    ~AsyncMapCache()
    {
        releaseMapValues();
    };
    //    AsyncMapCache(const AsyncMapCache& /*other*/): m_DataLoadingCount(0) {};
    AsyncMapCache(const AsyncMapCache & /*other*/) = delete ;
    AsyncMapCache &operator=(const AsyncMapCache & /*other*/) = delete ;
    /**
     * @brief 释放所有缓存的指针，并清空
     */
    void releaseMapValues()
    {
        //开始清理
        auto vs = m_Map.values();
        m_Map.clear();
        foreach(auto v, vs)
        {
            if(v)
            {
                //                qDebug() << __FUNCTION__ << k;
                //置空
                delete v;//
            }
        }
        m_WaitDataFuncs.clear();
    }
    QList<TKey>keys()
    {
        return  m_Map.keys();
    }
    /**
     * @brief 尝试获取，不存在则nullptr
     * @param k
     * @return
     */
    TValue *tryGet(const TKey &k)
    {
        if(m_Map.contains(k))
        {
            return m_Map.value(k);
        }
        return nullptr;
    }

    /**
     * @brief 获取
     * @param k
     * @param pullWhenNoCache 拉取数据项
     * @param retFunc 异步返回
     */
    void get(const TKey &k, const std::function<void(const TKey &, const std::function<void(TValue *)>&)> &pullWhenNoCache, const std::function<void(TValue *)> &retFunc)
    {
        if(m_Map.contains(k))//已经缓存或者正在拉取
        {
            auto v = m_Map.value(k);
            //            qDebug() << __FUNCTION__ << __LINE__ << v;
            if(v)
            {
                retFunc(v);    //
            }
            else
            {
                auto kf = qMakePair(k, retFunc);
                m_WaitDataFuncs << kf;
                //                qDebug() << __FUNCTION__ << __LINE__ << k;
            }
        }
        else//新建，并拉取
        {
            //插入空值，避免多次拉取
            m_Map.insert(k, nullptr);
            m_DataLoadingCount++;
            //            qDebug() << __FUNCTION__ << __LINE__ << m_DataLoadingCount;
            pullWhenNoCache(k, [ = ](TValue * remote)
            {
                //                qDebug() << __FUNCTION__ << __LINE__ << k << m_MapHasPullFinish.size();
                if(m_Map.contains(k))//当键存在时才更新
                {
                    //更新数据
                    m_Map.insert(k, remote);//
                    raiseWaitDataFuncs(k);
                }
                else//不存在则直接废弃数据
                {
                    if(remote)
                    {
                        delete remote;//
                        remote = nullptr;
                    }
                }
                retFunc(remote);//
                m_DataLoadingCount--;
                //                qDebug() << __FUNCTION__ << __LINE__ << m_DataLoadingCount;
            });
        }
    }
    /**
     * @brief contains
     * @param k
     * @return
     */
    bool contains(const TKey k)
    {
        return m_Map.contains(k);
    }
    void raiseWaitDataFuncs(TKey k)
    {
        //        qDebug() << __FUNCTION__ << __LINE__ << k;
        auto v = m_Map.value(k);
        if(!v)
        {
            return;
        }
        //        qDebug() << __FUNCTION__ << m_WaitDataFuncs.size();
        auto total = m_WaitDataFuncs.size();
        QList<int>lt;
        for(int i = 0; i < total; ++i)
        {
            auto kf = m_WaitDataFuncs[i];
            if(k == kf.first)
            {
                kf.second(v);
                lt << i;
            }
        }
        foreach(auto &item, lt)
        {
            m_WaitDataFuncs.removeAt(item);
        }
        //        qDebug() << __FUNCTION__ << m_WaitDataFuncs.size();
    }


    int getDataLoadingCount() const
    {
        return m_DataLoadingCount;
    }

private:
    /**
     * @brief m_Map
     */
    QMap<TKey, TValue *>m_Map;
    QList<QPair<TKey, std::function<void(TValue *)>>>m_WaitDataFuncs;
    std::atomic<int> m_DataLoadingCount{0};
};
}
#endif // ASYNCMAPCACHE_H
