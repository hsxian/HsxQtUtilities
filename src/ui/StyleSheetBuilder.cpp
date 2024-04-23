#include "StyleSheetBuilder.h"
#include <QDebug>
using namespace hsx;
StyleSheetBuilder &StyleSheetBuilder::setForeground(const QColor &color)
{
    _mapOfSheet["color"] =  color.name(QColor::NameFormat::HexArgb);
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setBackground(const QColor &color)
{
    _mapOfSheet["background-color"] =  color.name(QColor::NameFormat::HexArgb);
    //    qDebug() << "_mapOfSheet[\"background-color\"]" << _mapOfSheet["background-color"] << color.toRgb();
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setGridlineColor(const QColor &color)
{
    _mapOfSheet["gridline-color"] =  color.name(QColor::NameFormat::HexArgb);
    return *this;
}

StyleSheetBuilder &StyleSheetBuilder::setBackgroundImage(const QString &url)
{
    _mapOfSheet["border-image"] = QString("url(%1)").arg(url);
    return  *this;
}
StyleSheetBuilder &StyleSheetBuilder::setBackground(const QString &gradient)
{
    _mapOfSheet["background-color"] = gradient;
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setBackground()
{
    _mapOfSheet["background-color"] =  "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                       "stop: 0 #ff0000,"
                                       "stop: 0.2 #ffff00,"
                                       "stop: 0.35 #ffff00,"
                                       "stop: 0.45 #00ff00,"
                                       "stop: 0.5 #00ff00,"
                                       "stop: 0.6 #00ffff,"
                                       "stop: 0.75 #00ffff,"
                                       "stop: 1 #0000ff)";
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setFontSize(const int &size)
{
    _mapOfSheet["font-size"] =  QString::number(size) + "pt";
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setFontBold()
{
    _mapOfSheet["font-weight"] =  "bold";
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setFontFamily(const QString &family)
{
    _mapOfSheet["font-family"] =  family;
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
StyleSheetBuilder &StyleSheetBuilder::setBorderRadius(const int &top_left, const int &top_right, const int &bottom_left, const int &bottom_right)
{
    //    _mapOfSheet["border-radius"] = QString("%1px %2px %3px %4px !important").arg(left).arg(top).arg(right).arg(bottom);
    _mapOfSheet["border-top-left-radius"] = QString("%1px").arg(top_left);
    _mapOfSheet["border-top-right-radius"] = QString("%1px").arg(top_right);
    _mapOfSheet["border-bottom-left-radius"] = QString("%1px").arg(bottom_left);
    _mapOfSheet["border-bottom-right-radius"] = QString("%1px").arg(bottom_right);
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

StyleSheetBuilder &StyleSheetBuilder::setOutline(const int &w)
{
    _mapOfSheet["outline"] = QString("%1px").arg(w);
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setImage(const QString &url, const QString propertyName)
{
    _mapOfSheet[propertyName] = QString("url(%1)").arg(url);
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setImage(const QString &url, const QSize &size, const QString propertyName)
{
    _mapOfSheet[propertyName] = QString("url(%1);width:%2px;height:%3px")
                                .arg(url).arg(size.width()).arg(size.height());
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::set(const QString &key, const QString &vlaue)
{
    _mapOfSheet[key] = vlaue;
    //    qDebug() << __FUNCTION__ << vlaue << QJsonValue::fromVariant(vlaue) << _mapOfSheet[key].toString();
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::remove(const QString &key)
{
    _mapOfSheet.remove(key);
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::merge(const StyleSheetBuilder &athor)
{
    auto map = athor._mapOfSheet;
    auto keys = map.keys();
    foreach(auto item, keys)
    {
        _mapOfSheet[item] = map[item];
    }
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

QString StyleSheetBuilder::toStyleSheetOnlySelf(QWidget *wdt, QString state)
{
    return toStyleSheet("#" + wdt->objectName(), state);
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
StyleSheetBuilder &StyleSheetBuilder::setBorder(const int &left, const int &top, const int &right, const int &bottom, const QColor &color)
{
    auto cstr = color.name(QColor::NameFormat::HexArgb);
    //border-width:1px 0px 4px 0px;
    //border-color:rgb(0, 255, 255);
    //border-style:solid;
    _mapOfSheet["border-width"] = QString("%1px %2px %3px %4px").arg(top).arg(right).arg(bottom).arg(left);
    _mapOfSheet["border-color"] = cstr;
    _mapOfSheet["border-style"] = "solid";
    //    if(left > 0)
    //    {
    //        _mapOfSheet["border-left"] = QString("%1px solid %2").arg(left).arg(cstr);
    //    }
    //    else
    //    {
    //        remove("border-left");
    //    }
    //    if(top > 0)
    //    {
    //        _mapOfSheet["border-top"] = QString("%1px solid %2").arg(top).arg(cstr);
    //    }
    //    else
    //    {
    //        remove("border-top");
    //    }
    //    if(right > 0)
    //    {
    //        _mapOfSheet["border-right"] = QString("%1px solid %2").arg(right).arg(cstr);
    //    }
    //    else
    //    {
    //        remove("border-right");
    //    }
    //    if(bottom > 0)
    //    {
    //        _mapOfSheet["border-bottom"] = QString("%1px solid %2").arg(bottom).arg(cstr);
    //    }
    //    else
    //    {
    //        remove("border-bottom");
    //    }
    return  *this;
}
StyleSheetBuilder &StyleSheetBuilder::setBorder(const int &w, const QColor &color)
{
    _mapOfSheet["border"] = QString("%1px solid %2").arg(w).arg(color.name(QColor::NameFormat::HexArgb));
    return  *this;
}

StyleSheetBuilder &StyleSheetBuilder::setBorderNone()
{
    _mapOfSheet["border"] = QString("none");
    return  *this;
}
