#ifndef PAINTEVENTWIDGET_H
#define PAINTEVENTWIDGET_H

#include <QWidget>
#include <functional>
#include <HsxQtUtilitiesCore_global.h>

class QPaintEvent;
namespace hsx
{
class HSXQTUTILITIESCORE_EXPORT PaintEventWidget: public QWidget
{
public:
    PaintEventWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    std::function<void(QPaintEvent *event)>paintEventCallback;
    void swapChildren(QWidget *wdt);
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};
}
#endif // PAINTEVENTWIDGET_H
