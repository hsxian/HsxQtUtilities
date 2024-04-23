#ifndef QSQLITEHELPER_H
#define QSQLITEHELPER_H

#include <QString>
#include <QSqlDatabase>
#include <functional>

#include "HsxQtUtilities_global.h"
namespace hsx
{
/**
 * @brief The QSqliteHelper class
 * @author hsx
 */
class HSXQTUTILITIES_EXPORT QSqliteHelper
{
public:
    QSqliteHelper();
    /**
     * @brief 打开连接名或者数据库文件
     * @param connectionName
     * @param dbFile
     * @return
     */
    bool open(const QString &connectionName, const QString &dbFile);
    void close();
    /**
     * @brief 执行命令
     * @param cmd
     * @return
     */
    bool exec(const QString &cmd);
    /**
     * @brief 查询操作
     * @param sql
     * @param itemFunc 迭代每一行数据项
     * @return
     */
    int query(const QString &sql, const std::function<void(const QSqlQuery &)> &itemFunc);
private:
    bool m_IsOpen = false;
    QSqlDatabase m_Database;
};
}
#endif // QSQLITEHELPER_H
