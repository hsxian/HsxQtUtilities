#ifndef STRINGEX_H
#define STRINGEX_H

#include <QString>

#include <HsxQtUtilities_global.h>

namespace hsx
{
    class HSXQTUTILITIES_EXPORT StringEx
    {
    public:
        StringEx();
        QString trimTimeStringStart(const QString &str);
        QString limit(const QString& str, int size, const QString& suffix = "...");
        double calculateSimilarity(const QString& str1, const QString& str2);
        double calculateSimilarity2(const QString& str1, const QString& str2);
        int levenshteinDistance(const QString& str1, const QString& str2);
    };
}
#endif // STRINGEX_H
