#ifndef COMMONSORTFILTERPROXYMODEL_H
#define COMMONSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <functional>
#include <qdebug.h>
namespace hsx
{
template <class T>
class CommonSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    CommonSortFilterProxyModel(const std::function<bool(T *)> &acceptF, const std::function<bool(T *, T *)> &lessThanF, QObject *parent = Q_NULLPTR);
    void doFilter();
    void doSort();
    void setData(int arole = Qt::UserRole + 1);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
private:
    std::function<bool(T *)> _acceptFunc;
    std::function<bool(T *, T *)> _lessThanF;
    int m_DataRole = Qt::UserRole + 1;
};

template <class T>
CommonSortFilterProxyModel<T>::CommonSortFilterProxyModel(const std::function<bool(T *)> &acceptF, const std::function<bool(T *, T *)> &lessThanF, QObject *parent) : QSortFilterProxyModel(parent)
{
    _acceptFunc = acceptF;
    _lessThanF = lessThanF;
}

template <class T>
bool CommonSortFilterProxyModel<T>::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    auto itemData = (T *)index0.data((int)m_DataRole).value<void *>();
    return _acceptFunc(itemData);
}

template<class T>
inline bool CommonSortFilterProxyModel<T>::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    auto left = (T *)source_left.data((int)m_DataRole).value<void *>();
    auto right = (T *)source_right.data((int)m_DataRole).value<void *>();
    return _lessThanF(left, right);
}

template <class T>
void CommonSortFilterProxyModel<T>::doFilter()
{
    invalidateFilter();
}
template<class T>
inline void CommonSortFilterProxyModel<T>::doSort()
{
    invalidate();
    sort(0);
}

template<class T>
void CommonSortFilterProxyModel<T>::setData(int arole)
{
    m_DataRole = arole;
}
}
#endif // COMMONSORTFILTERPROXYMODEL_H
