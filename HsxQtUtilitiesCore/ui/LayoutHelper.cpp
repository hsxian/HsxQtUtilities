#include "LayoutHelper.h"
#include <QWidget>
using namespace hsx;

void LayoutHelper::clean(QLayout *ly)
{
    //    QLayoutItem* child;
    //    while(ly->count() && (child = ly->takeAt(0)) != 0)
    //    {
    //        if(child->widget())
    //        {
    //            child->widget()->setParent(nullptr);
    //        }
    //        delete child;
    //    }

    while(ly->count())
    {
        auto item = ly->takeAt(0);
        auto widget = item->widget();
        if(widget)
        {
            widget->setParent(nullptr);
            ly->removeWidget(widget);
        }
        ly->removeItem(item);
        if(widget)
        {
            delete  widget;
        }
        delete  item;
    }
}
