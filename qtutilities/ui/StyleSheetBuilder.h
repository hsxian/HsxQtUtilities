#ifndef STYLESHEETBUILDER_H
#define STYLESHEETBUILDER_H

#include <QColor>
#include <QJsonObject>
#include <QString>
#include <QFont>
#include <QWidget>


#include <qtutilities_global.h>

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT StyleSheetBuilder
    {
    public:
        StyleSheetBuilder();
        StyleSheetBuilder &setForeground(const QColor &color);
        StyleSheetBuilder &setBackground(const QColor &color);
        StyleSheetBuilder &setFontSize(const int &size);
        StyleSheetBuilder &setMargin(const int &left, const int &top, const int &right, const int &bottom);
        StyleSheetBuilder &setPadding(const int &left, const int &top, const int &right, const int &bottom);
        StyleSheetBuilder &setBorder(const int &left, const int &top, const int &right, const int &bottom);
        StyleSheetBuilder &setBorderRadius(const int &left, const int &top, const int &right, const int &bottom);
        StyleSheetBuilder &setHeight(const int &h);
        StyleSheetBuilder &setWidth(const int &w);
        StyleSheetBuilder &clean();
        QString toStyleSheet();
        QString toStyleSheet(QWidget *wdt, const QString &state = "");
        QString toStyleSheet(const QString &selectionName, const QString &state = "");
        StyleSheetBuilder &setBorder(const int &w, const QColor &color);
    private:
        QJsonObject _mapOfSheet;
    };
}
#endif // STYLESHEETBUILDER_H
