#include "StyleSheetBuilder.h"

using namespace hsx;

StyleSheetBuilder::StyleSheetBuilder()
{

}

StyleSheetBuilder &StyleSheetBuilder::setForeground(const QColor &color)
{
    _mapOfSheet["color"] =  color.name(QColor::NameFormat::HexArgb);
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setBackground(const QColor &color)
{
    _mapOfSheet["background-color"] =  color.name(QColor::NameFormat::HexArgb);
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setFontSize(const int &size)
{
    _mapOfSheet["font-size"] =  QString::number(size) + "pt";
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setMargin(const int &left, const int &top, const int &right, const int &bottom)
{
    _mapOfSheet["margin-left"] =  QString::number(left) + "px";
    _mapOfSheet["margin-top"] =  QString::number(top) + "px";
    _mapOfSheet["margin-right"] =  QString::number(right) + "px";
    _mapOfSheet["margin-bottom"] =  QString::number(bottom) + "px";
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setPadding(const int &left, const int &top, const int &right, const int &bottom)
{
    _mapOfSheet["padding-left"] =  QString::number(left) + "px";
    _mapOfSheet["padding-top"] =  QString::number(top) + "px";
    _mapOfSheet["padding-right"] =  QString::number(right) + "px";
    _mapOfSheet["padding-bottom"] =  QString::number(bottom) + "px";
    return  *this;
}
StyleSheetBuilder &StyleSheetBuilder::setBorderRadius(const int &left, const int &top, const int &right, const int &bottom)
{
    _mapOfSheet["border-radius"] = QString("%1px %2px %3px %4px !important").arg(left).arg(top).arg(right).arg(bottom);
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setHeight(const int &h)
{
    _mapOfSheet["height"] = QString("%1px").arg(h);
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setWidth(const int &w)
{
    _mapOfSheet["width"] = QString("%1px").arg(w);
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::clean()
{
    while(_mapOfSheet.size())
    {
        _mapOfSheet.remove(_mapOfSheet.keys().at(0));
    }
    return  *this;
}

QString StyleSheetBuilder::toStyleSheet()
{
    QString ret;
    foreach(auto key, _mapOfSheet.keys())
    {
        ret += QString("%1:%2;").arg(key).arg(_mapOfSheet[key].toString());
    }
    return  ret;
}

QString StyleSheetBuilder::toStyleSheet(QWidget *wdt, const QString &state)
{
    QString ret = toStyleSheet();
    if(wdt)
    {
        return QString("%1%2{%3}")
               .arg(wdt->metaObject()->className())
               .arg(state.isEmpty() ? "" : QString(":%1").arg(state))
               .arg(ret);
    }
    return  ret;
}

QString StyleSheetBuilder::toStyleSheet(const QString &selectionName, const QString &state)
{
    QString ret = toStyleSheet();

    return QString("%1%2{%3}")
           .arg(selectionName)
           .arg(state.isEmpty() ? "" : QString(":%1").arg(state))
           .arg(ret);

    return  ret;
}

StyleSheetBuilder &StyleSheetBuilder::setBorder(const int &left, const int &top, const int &right, const int &bottom)
{
    _mapOfSheet["border"] = QString("%1px %2px %3px %4px").arg(left).arg(top).arg(right).arg(bottom);
    return  *this;
}
StyleSheetBuilder &StyleSheetBuilder::setBorder(const int &w, const QColor &color)
{
    _mapOfSheet["border"] = QString("%1px solid %2").arg(w).arg(color.name(QColor::NameFormat::HexArgb));
    return  *this;
}
