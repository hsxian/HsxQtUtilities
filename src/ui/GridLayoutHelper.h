#ifndef GRIDLAYOUTHELPER_H
#define GRIDLAYOUTHELPER_H

#include <HsxQtUtilities_global.h>
#include <QGridLayout>
namespace hsx
{
class HSXQTUTILITIES_EXPORT GridLayoutHelper
{
public:
    GridLayoutHelper(QGridLayout *grid);
    void setFixedColumnWidth(int column, int w);
private:
    QGridLayout *m_Grid = nullptr;
};
}
#endif // GRIDLAYOUTHELPER_H
