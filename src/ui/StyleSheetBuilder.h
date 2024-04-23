#ifndef STYLESHEETBUILDER_H
#define STYLESHEETBUILDER_H

#include <QColor>
#include <QJsonObject>
#include <QString>
#include <QFont>
#include <QWidget>


#include "HsxQtUtilities_global.h"
namespace hsx
{
class HSXQTUTILITIES_EXPORT StyleSheetBuilder
{
public:
    StyleSheetBuilder() = default;
    StyleSheetBuilder &setForeground(const QColor &color);
    StyleSheetBuilder &setBackground(const QColor &color);
    StyleSheetBuilder &setGridlineColor(const QColor &color);
    StyleSheetBuilder &setBackground(const QString &gradient);
    StyleSheetBuilder &setBackground();
    StyleSheetBuilder &setFontSize(const int &size);
    StyleSheetBuilder &setFontBold();
    StyleSheetBuilder &setFontFamily(const QString &family);
    StyleSheetBuilder &setMargin(const int &left, const int &top, const int &right, const int &bottom);
    StyleSheetBuilder &setPadding(const int &left, const int &top, const int &right, const int &bottom);
    StyleSheetBuilder &setBorder(const int &left, const int &top, const int &right, const int &bottom);
    StyleSheetBuilder &setBorderRadius(const int &top_left, const int &top_right, const int &bottom_left, const int &bottom_right);
    StyleSheetBuilder &setHeight(const int &h);
    StyleSheetBuilder &setWidth(const int &w);
    StyleSheetBuilder &setOutline(const int &w);
    StyleSheetBuilder &setImage(const QString &url, const QString propertyName = "image");
    StyleSheetBuilder &setImage(const QString &url, const QSize &size, const QString propertyName = "image");
    StyleSheetBuilder &clean();
    QString toStyleSheet();
    QString toStyleSheet(QWidget *wdt, const QString &state = "");
    QString toStyleSheetOnlySelf(QWidget *wdt, QString state = "");
    QString toStyleSheet(const QString &selectionName, const QString &state = "");
    StyleSheetBuilder &setBorder(const int &w, const QColor &color);
    StyleSheetBuilder &setBorder(const int &left, const int &top, const int &right, const int &bottom, const QColor &color);
    StyleSheetBuilder &setBorderNone();
    StyleSheetBuilder &set(const QString &key, const QString &vlaue);
    StyleSheetBuilder &remove(const QString &key);
    StyleSheetBuilder &merge(const StyleSheetBuilder &athor);
    StyleSheetBuilder &setBackgroundImage(const QString &url);
private:
    QJsonObject _mapOfSheet;
};
}
#endif // STYLESHEETBUILDER_H
