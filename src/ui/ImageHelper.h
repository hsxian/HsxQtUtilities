#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H

#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <HsxQtUtilities_global.h>
namespace hsx
{
/**
 * @brief The ImageHelper class
 * @author hsx
 */
class HSXQTUTILITIES_EXPORT ImageHelper
{
public:
    ImageHelper();
    /**
     * @brief scaleImageToAdaptLabel
     * @param lbl
     * @param img
     */
    void scaleImageToAdaptLabel(QLabel *lbl, QImage *img);
    /**
     * @brief scaleImageToAdaptLabel
     * @param lbl
     * @param img
     */
    void scaleImageToAdaptLabel(QLabel *lbl, const QPixmap &img);
    QImage ByteArrayToImage(QByteArray array);
    void setImageAsBackground(QImage *img, QWidget *wdt, const QSize &scale);
    void setImageToButton(QPushButton *btn, QImage *img);
    void imageToBase64(const QPixmap &img, const QString &ext, QByteArray &ret);
    void base64ToImage(const QString &base64, QPixmap &ret);
    void base64ToImage(const QByteArray &base64, QPixmap &ret);
};
}
#endif // IMAGEHELPER_H
