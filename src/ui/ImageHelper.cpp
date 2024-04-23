#include "ImageHelper.h"

#include <QBuffer>
#include <QImageReader>
#include <QDebug>
using namespace hsx;
ImageHelper::ImageHelper()
{

}
void ImageHelper::scaleImageToAdaptLabel(QLabel *lbl, QImage *img)
{
    if(!img)//
    {
        return;//
    }
    auto pix = QPixmap::fromImage(*img);//
    pix.scaled(lbl->size(), Qt::KeepAspectRatio);//
    scaleImageToAdaptLabel(lbl, pix);//
}
void ImageHelper::scaleImageToAdaptLabel(QLabel *lbl, const QPixmap &img)
{
    lbl->setScaledContents(true);//
    lbl->setPixmap(img);//
}
QImage ImageHelper::ByteArrayToImage(QByteArray array)
{
    //
    QBuffer buffer(&array);//
    buffer.open(QIODevice::ReadOnly);//
    QImageReader reader(&buffer, "JPG");//
    QImage img = reader.read();//
    if(img.isNull())//
    {
        return QImage();//
    }
    return img;
}

void ImageHelper::setImageAsBackground(QImage *img, QWidget *wdt, const QSize &scale)
{
    QPalette pl(wdt->palette());//
    auto newImg = img->scaled(scale);//

#if (QT_VERSION <= QT_VERSION_CHECK(6,0,0))
    pl.setBrush(QPalette::Background, QBrush(newImg));//
#else
    pl.setBrush(QPalette::Window, QBrush(newImg));//
#endif
    wdt->setPalette(pl);//
    wdt->setAutoFillBackground(true);//
}
void ImageHelper::setImageToButton(QPushButton *btn, QImage *img)
{
    auto pix = QPixmap::fromImage(*img);//
    btn->setIcon(pix);//
}

void ImageHelper::imageToBase64(const QPixmap &img, const QString &ext, QByteArray &ret)
{
    //    qDebug() << __FUNCTION__ << img << ext.toStdString().c_str();
    QByteArray ba;
    QBuffer bf(&ba);
    img.save(&bf, ext.toStdString().c_str());
    //    qDebug() << __FUNCTION__ << ba.mid(10);
    ret = ba.toBase64();
    //    qDebug() << __FUNCTION__ << ret.size();
    bf.close();
}


void ImageHelper::base64ToImage(const QByteArray &base64, QPixmap &ret)
{
    auto ret_by = QByteArray::fromBase64(base64);
    ret.loadFromData(ret_by);
}
void ImageHelper::base64ToImage(const QString &base64, QPixmap &ret)
{
    base64ToImage(base64.toLocal8Bit(), ret);
}
