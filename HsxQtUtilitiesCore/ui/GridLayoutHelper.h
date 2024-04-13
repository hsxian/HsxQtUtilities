#ifndef GRIDLAYOUTHELPER_H
#define GRIDLAYOUTHELPER_H

#include <HsxQtUtilitiesCore_global.h>
#include <QGridLayout>
namespace hsx
{
class HSXQTUTILITIESCORE_EXPORT GridLayoutHelper
{
public:
    GridLayoutHelper(QGridLayout *grid);
    void setFixedColumnWidth(int column, int w);
private:
    QGridLayout *m_Grid = nullptr;
};
}
#endif // GRIDLAYOUTHELPER_H
