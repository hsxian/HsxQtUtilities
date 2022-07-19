#ifndef QSTANDARDITEMMODELHELPER_H
#define QSTANDARDITEMMODELHELPER_H

#include <QVariant>
#include <QStandardItem>
#include <qtutilities_global.h>

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT QStandardItemModelHelper
    {
    public:
        QStandardItemModelHelper();
        QList<QStandardItem *>generateQStandardItems(QStringList &data, const QString &accessibleText = "");
        QList<QStandardItem *>generateQStandardItems(QStringList &data, const QVariant &qv, const QString &accessibleText = "");
        QList<QStandardItem *>generateQStandardItems(QStringList &data, const QVariant &qv, const int role = Qt::UserRole + 1);
        QList<QStandardItem *>columnItems(const QStandardItemModel &m, int column = 0);
        void setDateToQStandardItems(QList<QStandardItem *> &qitems, const QVariant &qv, const int role);
    };
}
#endif // QSTANDARDITEMMODELHELPER_H
