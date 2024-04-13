#include "ExclusiveWindow.h"
#include <QDebug>
using namespace hsx;
void add(QMap<QString, QWidgetList *> &map, const QString &groupName, QWidget *wgt)
{
    auto lt = map[groupName];
    if(!lt)
    {
        lt = new QWidgetList();
        map[groupName] = lt;
    }
    if(lt->contains(wgt))
    {
        return;
    }
    lt->push_back(wgt);
}

ExclusiveWindow::ExclusiveWindow()
{

}

void ExclusiveWindow::registerWidget(const QString &groupName, QWidget *wgt)
{
    if(!wgt)
    {
        return;
    }
    add(m_MapOfWidgets, groupName, wgt);
    qDebug() << __FUNCTION__  << groupName << m_MapOfWidgets[groupName]->size();
}

void ExclusiveWindow::exclusiveShow(const QString &groupName, QWidgetList wgts)
{
    auto ltp = m_MapOfWidgets[groupName];
    if(!ltp)
    {
        return;
    }
    auto lt = *ltp;
    foreach(auto item, lt)
    {
        //        qDebug() << __FUNCTION__  << item->objectName() << item->isVisible() << item->isHidden();
        if(!wgts.contains(item) && !item->isHidden())
        {
            //            qDebug() << __FUNCTION__ << "hide" << item->objectName();
            execAnimation(groupName, item,  false);
            if(!existHideAnimation(item))
            {
                item->hide();
            }
        }
    }
    foreach(auto item, wgts)
    {
        if(item->isHidden())
        {
            //            qDebug() << __FUNCTION__ << "show" << item->objectName();
            item->show();
            execAnimation(groupName, item,  true);
        }
    }
}

void ExclusiveWindow::exclusiveShow(const QString &groupName, const QString &objectName)
{
    exclusiveShow(groupName, QStringList() << objectName);
}
void ExclusiveWindow::exclusiveShow(const QString &groupName, const QStringList &objectNames)
{
    auto ltp = m_MapOfWidgets[groupName];
    if(!ltp)
    {
        return;
    }
    auto lt = *ltp;
    foreach(auto item, lt)
    {
        //        qDebug() << __FUNCTION__  << item->objectName() << item->isVisible() << item->isHidden();
        auto name = item->objectName();
        if(! objectNames.contains(name) && !item->isHidden())
        {
            //            qDebug() << __FUNCTION__ << "hide" << item->objectName();
            execAnimation(groupName, item,  false);
            if(!existHideAnimation(item))
            {
                item->hide();
            }
        }
        else if(objectNames.contains(name) && item->isHidden())
        {
            //            qDebug() << __FUNCTION__ << "show" << item->objectName();
            execAnimation(groupName, item, true);
            item->show();
        }
    }
}

void ExclusiveWindow::setCreateAnimationFunc(const std::function<AnimationGroup *(const QString &, QWidget *)> &func)
{
    m_CreateAnimationFunc = func;
}
bool ExclusiveWindow::existHideAnimation(QWidget *wgt)
{
    auto g = m_MapOfAnimation[wgt];
    return g && g->hides.size();
}

void ExclusiveWindow::execAnimation(const QString &groupName, QWidget *wgt, bool showOrHide)
{
    auto g = m_MapOfAnimation[wgt];
    if(!g && m_CreateAnimationFunc)
    {
        g = m_CreateAnimationFunc(groupName, wgt);
        if(g)
        {
            m_MapOfAnimation[wgt] = g;
        }
    }
    if(!g)
    {
        return;
    }
    if(showOrHide)
    {
        foreach(auto item, g->shows)
        {
            //            qDebug() << __FUNCTION__ << "show" << wgt->objectName() << item->startValue() << item->endValue();
            item->start();
        }
    }
    else
    {
        foreach(auto item, g->hides)
        {
            //            qDebug() << __FUNCTION__ << "hide" << wgt->objectName() << item->startValue() << item->endValue();
            item->start();
        }
    }
}
void ExclusiveWindow::exclusiveShow(const QString &groupName, QWidget *wgt)
{
    exclusiveShow(groupName, QWidgetList() << wgt);
}
