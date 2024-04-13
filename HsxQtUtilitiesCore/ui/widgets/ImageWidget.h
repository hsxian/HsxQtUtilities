#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <functional>
#include "HsxQtUtilitiesCore_global.h"
namespace hsx
{
class HSXQTUTILITIESCORE_EXPORT ImageWidget: public QWidget
{
public:
    ImageWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    QColor background = Qt::transparent;
    QImage *img = nullptr;
    std::function<void(QWidget *, QRect &)>getImageQRectFunc;
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
private:
};
}
#endif // IMAGEWIDGET_H
