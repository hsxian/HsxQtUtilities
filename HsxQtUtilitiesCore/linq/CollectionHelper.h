#pragma once
#ifndef COLLECTIONHELPER_H
#define COLLECTIONHELPER_H

#include <QList>
#include <QString>
#include <functional>
#include <HsxQtUtilitiesCore_global.h>

namespace hsx
{
class HSXQTUTILITIESCORE_EXPORT  CollectionHelper
{
public:
    CollectionHelper();
    enum MatchResult
    {
        Match,
        needAdd,
        needRemove,
    };

    template<class Tl, class Tf>
    void followNotChagedList2(const QList<Tl> &leader,
                              const QList<Tf> &flowwer,
                              const std::function<QString(Tl)> &keyLFunc,
                              const std::function<QString(Tf)> &keyFFunc,
                              const std::function<void(Tl)> &needAddFunc,
                              const  std::function<void(Tf)> &needRemoveFunc)
    {
        foreach(auto l, leader)
        {
            auto keyL = keyLFunc(l);
            bool needAdd = true;
            foreach(auto f, flowwer)
            {
                auto keyF = keyFFunc(f);
                if(keyL == keyF)
                {
                    needAdd = false;
                    break;
                }
            }
            if(needAdd)
            {
                needAddFunc(l);
            }
        }
        for(int i = 0; i < flowwer.count(); i++)
        {
            auto f = flowwer.at(i);
            auto keyF = keyFFunc(f);
            auto needRemove = true;
            foreach(auto l, leader)
            {
                auto keyL = keyLFunc(l);
                if(keyL == keyF)
                {
                    needRemove = false;
                    break;
                }
            }
            if(needRemove)
            {
                needRemoveFunc(f);
            }
        }
    }

    template<class Tl, class Tf>
    void followNotChagedList(const QList<Tl> &leader,
                             const QList<Tf> &flowwer,
                             const std::function<QString(Tl)> &keyLFunc,
                             const std::function<QString(Tf)> &keyFFunc,
                             const std::function<void(CollectionHelper::MatchResult, Tl, Tf)> &matchResultFunc)
    {
        foreach(auto l, leader)
        {
            auto keyL = keyLFunc(l);
            bool needAdd = true;
            foreach(auto f, flowwer)
            {
                auto keyF = keyFFunc(f);
                if(keyL == keyF)
                {
                    matchResultFunc(CollectionHelper::Match, l, f);
                    needAdd = false;
                    break;
                }
            }
            if(needAdd)
            {
                matchResultFunc(CollectionHelper::needAdd, l, Tf());
            }
        }
        for(int i = 0; i < flowwer.count(); i++)
        {
            auto f = flowwer.at(i);
            auto keyF = keyFFunc(f);
            auto needRemove = true;
            foreach(auto l, leader)
            {
                auto keyL = keyLFunc(l);
                if(keyL == keyF)
                {
                    needRemove = false;
                    break;
                }
            }
            if(needRemove)
            {
                matchResultFunc(CollectionHelper::needRemove, Tl(), f);
            }
        }
    }
    template<class T>
    T firstOrDefault(const QList<T>lt, const std::function<bool(T)> &pFunc)
    {
        foreach(auto item, lt)
        {
            if(pFunc(item))
            {
                return item;
            }
        }
        return T();
    }
};
}
#endif // COLLECTIONHELPER_H
