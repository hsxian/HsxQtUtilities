#include "StringEx.h"
#include <QDebug>

using namespace hsx;

StringEx::StringEx()
{

}

QString StringEx::trimTimeStringStart(const QString &str)
{
    auto temp = str;
    while(temp.startsWith("0") && temp.size() > 2)
    {
        temp = temp.mid(2);
    }
    return temp;
}
QString StringEx::limit(const QString& str, int size, const QString& suffix)
{
    if(str.size() <= size)
    {
        return str;
    }
    return str.left(size) + suffix;
}
inline QSet<QChar>toSet(const QString& str1)
{
    QSet<QChar>set;
    foreach(auto item, str1)
    {
        set << item;
    }
    return set;
}
double StringEx::calculateSimilarity(const QString& str1, const QString& str2)
{
    int intersectionCount = 0;
    int unionCount = str1.size() + str2.size();
    foreach(auto item, str1)
    {
        if(str2.contains(item))
        {
            intersectionCount++;
            unionCount--;
        }
    }
    auto sim = (double)intersectionCount / unionCount;
    return sim;
}
double StringEx::calculateSimilarity2(const QString& str1, const QString& str2)
{
    int len1 = str1.length();
    int len2 = str2.length();

    QVector<QVector<int>> dp(len1 + 1, QVector<int>(len2 + 1));
    //        int dp[len1 + 1][len2 + 1];

    for(int i = 0; i <= len1; ++i)
    {
        //        QStringList lt;
        for(int j = 0; j <= len2; ++j)
        {
            if(i == 0)
            {
                dp[i][j] = j;
            }
            else if(j == 0)
            {
                dp[i][j] = i;
            }
            else if(str1[i - 1] == str2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else
            {
                dp[i][j] = 1 + qMin(dp[i - 1][j - 1], qMin(dp[i - 1][j], dp[i][j - 1]));
            }
            //            lt << QString::number(dp[i][j]);
        }
        //        qDebug() << __FUNCTION__ << lt.join(' ');
    }
    return 1 - (double)dp[len1][len2] / qMax(len1, len2);
}

int StringEx::levenshteinDistance(const QString& str1, const QString& str2)
{
    int m = str1.length();
    int n = str2.length();
    QVector<int> dp(m + 1, 0);
    dp[0] = n;
    for(int i = 1; i <= m; ++i)
    {
        dp[i] = i;
        for(int j = 1; j <= n; ++j)
        {
            if(str1[i - 1] == str2[j - 1])
            {
                dp[i] = qMin(dp[i], dp[i - 1]);
            }
            else
            {
                dp[i] = qMin(dp[i], dp[i - 1] + 1);
            }
        }
    }
    return dp[m];
}
