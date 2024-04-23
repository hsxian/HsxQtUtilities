#include "GridLayoutHelper.h"
#include <QWidget>
using namespace hsx;

GridLayoutHelper::GridLayoutHelper(QGridLayout *grid)
{
    m_Grid = grid;
}

void GridLayoutHelper::setFixedColumnWidth(int column, int w)
{
    for(int i = 0; i < m_Grid->rowCount(); ++i)
    {
        auto item = m_Grid->itemAtPosition(i, column);
        if(item)
        {
            if(item->widget())
            {
                item->widget()->setFixedWidth(w);
            }
            if(item->layout() && item->layout()->widget())
            {
                //                item->layout()->widget()->setFixedWidth(w);
            }
        }
    }
}
