#ifndef MODELUPDATETOVIEWHELP_H
#define MODELUPDATETOVIEWHELP_H

#include "HsxQtUtilitiesCore_global.h"

#include <QList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QAbstractItemView>
#include <functional>
namespace hsx
{
/**
 * @brief The ModelUpdateToViewHelp class
 * @author hsx
 */
template<class T>
class HSXQTUTILITIESCORE_EXPORT ModelUpdateToViewHelp
{
public:
    /**
     * @brief 同步
     * @param source
     * @param m
     * @param u
     */
    void sync(QList<T *> &source, QStandardItemModel &m, QAbstractItemView *u)
    {
        auto uiCount = m.rowCount();
        auto dataCount = source.count();
        //update
        auto updateCount = qMin(uiCount, dataCount);
        for(int i = 0; i < updateCount; ++i)
        {
            auto idx = m.index(i, 0);
            auto qitem = m.itemFromIndex(idx);
            auto data = source.at(i);
            qitem->setData(QVariant::fromValue((void *)data));
            auto wgt = u->indexWidget(idx);
            wgt->show();
            m_OperationFunc(data, qitem, wgt);
        }
        //add
        for(int i = updateCount; i < dataCount; ++i)
        {
            auto data = source.at(i);
            auto qitem = new QStandardItem();
            qitem->setData(QVariant::fromValue((void *)data));
            auto idx = m.indexFromItem(qitem);
            m_OperationFunc(data, qitem, idx);
        }
        //hide
        for(int i = updateCount; i < uiCount; ++i)
        {
            auto idx = m.index(i, 0);
            auto qitem = m.itemFromIndex(idx);
            auto wgt = u->indexWidget(idx);
            wgt->hide();
            m_OperationHideFunc(qitem, wgt);
        }
    }

    int DataRole() const
    {
        return m_DataRole;
    }

    void setDataRole(int dataRole)
    {
        m_DataRole = dataRole;
    }

    std::function<void (T *, QStandardItem *, QWidget *)> OperationUpdateFunc() const
    {
        return m_OperationUpdateFunc;
    }

    void setOperationUpdateFunc(const std::function<void (T *, QStandardItem *, QWidget *)> &operationUpdateFunc)
    {
        m_OperationUpdateFunc = operationUpdateFunc;
    }

    std::function<void (T *, QStandardItem *, QModelIndex &)> OperationAddFunc() const
    {
        return m_OperationAddFunc;
    }

    void setOperationAddFunc(const std::function<void (T *, QStandardItem *, QModelIndex &)> &operationAddFunc)
    {
        m_OperationAddFunc = operationAddFunc;
    }

    std::function<void (QStandardItem *, QWidget *)> OperationHideFunc() const
    {
        return m_OperationHideFunc;
    }

    void setOperationHideFunc(const std::function<void (QStandardItem *, QWidget *)> &operationHideFunc)
    {
        m_OperationHideFunc = operationHideFunc;
    }


private:
    int m_DataRole = Qt::UserRole + 1;
    std::function<void(T *, QStandardItem *, QWidget *)>m_OperationUpdateFunc;
    std::function<void(T *, QStandardItem *, QModelIndex &)>m_OperationAddFunc;
    std::function<void(QStandardItem *, QWidget *)>m_OperationHideFunc;
};
}
#endif // MODELUPDATETOVIEWHELP_H
