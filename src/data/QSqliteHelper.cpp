#include "QSqliteHelper.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

#include "io/IoHelper.h"
using namespace hsx;
QSqliteHelper::QSqliteHelper()
{

}

void QSqliteHelper::close()
{
    if(m_IsOpen)
    {
        m_Database.close();
    }
}

bool QSqliteHelper::exec(const QString &cmd)
{
    QSqlQuery query(m_Database);
    auto ret = query.exec(cmd);
    if(!ret)
    {
        qDebug() << __FUNCTION__ << __LINE__ << m_Database.lastError();
    };
    return ret;
}

int QSqliteHelper::query(const QString &sql, const std::function<void(const QSqlQuery &)> &itemFunc)
{
    QSqlQuery query(m_Database);
    auto ret = query.exec(sql);
    if(ret)
    {
        while(query.next())
        {
            itemFunc(query);
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << __LINE__ << m_Database.lastError();
    }
    return ret;
}

bool QSqliteHelper::open(const QString &connectionName, const QString &dbFile)
{
    if(m_Database.contains(connectionName))
    {
        m_Database = QSqlDatabase::database(connectionName);
    }
    else
    {
        IoHelper ih;
        m_Database = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        ih.mkDirWhenNotExistWithFile(dbFile);
        m_Database.setDatabaseName(dbFile);
    }
    m_IsOpen = m_Database.open();
    if(!m_IsOpen)
    {
        qDebug() << __FUNCTION__ << __LINE__ << m_Database.lastError();
    }

    return m_IsOpen;
}
