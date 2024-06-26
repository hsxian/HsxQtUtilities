﻿#pragma once
#pragma execution_character_set("utf-8")

#ifndef QROUNDPROGRESSBAR_H
#define QROUNDPROGRESSBAR_H

#include <QWidget>

#include "../../common/CommonDefine.h"

#include <HsxQtUtilities_global.h>

namespace hsx
{
    class HSXQTUTILITIES_EXPORT  QRoundProgressBar : public QWidget
    {
        Q_OBJECT

    public:
        //进度条形状
        enum BarStyle
        {
            //圆环状
            StyleDonut = 0,
            //圆饼状
            StylePie,
            //圆线状
            StyleLine
        };
        QRoundProgressBar(QWidget *parent = 0, BarStyle style = StyleLine);
        ~QRoundProgressBar();

    public:
        //设置扫描弧度起始角度
        void setStartAngle(double position);

        //设置轮廓画笔的宽度
        void setOutlinePenWidth(double penWidth);

        //设置画笔宽度
        void setDataPenWidth(double penWidth);

        //设置进度条精度 88.88%,几位小数
        void setDecimals(int count);

        //设置进度条风格
        void setBarStyle(BarStyle style);

        //设置范围
        void setRange(double min, double max);

        //设置当前值
        void setValue(int val);

        //设置当前值
        void setValue(double val);

        //设置最小值
        void setMinimum(double min);

        //设置最大值
        void setMaximum(double max);

        PropertyBuilderByNameAndInit(QColor, Background, private, Qt::transparent);
        PropertyBuilderByNameAndInit(QColor, Background4InnerCircle, private, Qt::transparent);
        PropertyBuilderByNameAndInit(QColor, Color4Value, private, QColor("#2F8DED"));
        PropertyBuilderByNameAndInit(QColor, Color4Base, private, QColor("#DEE3E7"));
        PropertyBuilderByNameAndInit(QColor, Color4Text, private, QColor("#606266"));
        PropertyBuilderByNameAndInit(bool, PercentageSymbolVisible, private, true);
        PropertyBuilderByNameAndInit(QString, Text, private, "");
        PropertyBuilderByNameAndInit(int, FontSize, private, 20);

    protected:
        void paintEvent(QPaintEvent *event);

        //画基础图形
        void drawBase(QPainter &p, const QRectF &baseRect, const QRectF &innerRect);

        //根据值画出进度条
        void drawValue(QPainter &p, const QRectF &baseRect, double value, double arcLength, const QRectF &innerRect
                       , double innerDiameter);

        //画中心文字
        void drawText(QPainter &p, const QRectF &innerRect, double value);

    private:
        double   m_min;                                 //最小值
        double   m_max;                                 //最大值
        double   m_value;                               //当前值
        BarStyle m_barStyle;                            //进度条风格
        int      m_decimals;                            //进度条精度
        double   m_startAngel;                          //扫描弧度起始角度
        double   m_outlinePenWidth;                     //轮廓画笔的宽度
        double   m_dataPenWidth;                        //画笔宽度
    };
}
#endif // QROUNDPROGRESSBAR_H
