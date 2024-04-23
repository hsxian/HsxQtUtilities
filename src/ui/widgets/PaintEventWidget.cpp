#include "PaintEventWidget.h"
#include <QLayout>
using namespace hsx;
PaintEventWidget::PaintEventWidget(QWidget *parent, Qt::WindowFlags f): QWidget(parent, f)
{

}

void PaintEventWidget::swapChildren(QWidget *wdt)
{
    if(!wdt->layout())
    {
        return;
    }
    auto ly = new QHBoxLayout(this);
    auto m = wdt->layout()->contentsMargins();
    ly->setContentsMargins(m);
    ly->setSpacing(wdt->layout()->spacing());
    setLayout(ly);
    while(wdt->layout()->count())
    {
        auto item = wdt->layout()->takeAt(0);
        ly->addItem(item);
    }
    wdt->layout()->addWidget(this);
}

void PaintEventWidget::paintEvent(QPaintEvent *event)
{
    if(paintEventCallback)
    {
        paintEventCallback(event);
    }
}
