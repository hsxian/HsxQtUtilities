#include "QStandardItemModelHelper.h"

using namespace hsx;

QStandardItemModelHelper::QStandardItemModelHelper()
{

}

QList<QStandardItem *> QStandardItemModelHelper::generateQStandardItems(QStringList &data, const QString &accessibleText)
{
    QList<QStandardItem *>ret;
    foreach(auto txt, data)
    {
        auto qitem = new QStandardItem();
        qitem->setAccessibleText(accessibleText);
        qitem->setText(txt);
        qitem->setToolTip(txt);
        ret.push_back(qitem);
    }
    return ret;
}

QList<QStandardItem *> QStandardItemModelHelper::generateQStandardItems(QStringList &data, const QVariant &qv, const QString &accessibleText)
{
    QList<QStandardItem *>ret;
    foreach(auto txt, data)
    {
        auto qitem = new QStandardItem();
        qitem->setAccessibleText(accessibleText);
        qitem->setText(txt);
        qitem->setToolTip(txt);
        qitem->setData(qv);
        ret.push_back(qitem);
    }
    return ret;
}

QList<QStandardItem *> QStandardItemModelHelper::generateQStandardItems(QStringList &data, const QVariant &qv, const int role)
{
    QList<QStandardItem *>ret;
    foreach(auto txt, data)
    {
        auto qitem = new QStandardItem();
        qitem->setText(txt);
        qitem->setToolTip(txt);
        qitem->setData(qv, role);
        ret.push_back(qitem);
    }
    return ret;
}

void QStandardItemModelHelper::setDateToQStandardItems(QList<QStandardItem *> &qitems, const QVariant &qv, const int role)
{
    foreach(auto qitem, qitems)
    {
        qitem->setData(qv, role);
    }
}
QList<QStandardItem *> QStandardItemModelHelper::columnItems(const QStandardItemModel &m, int column)
{
    QList<QStandardItem *>ret;
    for(int i = 0; i < m.rowCount(); ++i)
    {
        ret << m.item(i, column);
    }
    return ret;
}
