#include "ImageWidget.h"

#include <QPainter>
#include <QPixmap>
#include <QLayout>
#include <QDebug>
using namespace hsx;
ImageWidget::ImageWidget(QWidget *parent, Qt::WindowFlags f)
{

}
void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter pr(this);
    pr.fillRect(QRect(0, 0, width(), height()), background);
    if(img)
    {
        QRect rect;
        if(getImageQRectFunc)
        {
            getImageQRectFunc(this, rect);
        }
        else
        {
            rect.setX(0);
            rect.setY(0);
            rect.setWidth(width());
            rect.setHeight(height());
        }
        pr.drawImage(rect, *img);
    }
}
