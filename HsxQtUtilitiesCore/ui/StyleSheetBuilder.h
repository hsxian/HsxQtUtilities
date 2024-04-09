#ifndef STYLESHEETBUILDER_H
#define STYLESHEETBUILDER_H

#include <QColor>
#include <QJsonObject>
#include <QString>
#include <QFont>
#include <QWidget>


#include <HsxQtUtilitiesCore_global.h>

namespace hsx
{
class HSXQTUTILITIESCORE_EXPORT StyleSheetBuilder
{
public:
    enum StateTypes
    {
        active,//此状态在widget驻留在活动窗口中时设置。
        adjoins_item,//此状态在QTreeView的branch与项相邻时设置。
        alternate,//当QAbstractItemViewternatingRowColors()设置为true时，将为绘制QAbstractItemView的行时的每隔一行设置此状态。
        bottom,//该项目位于底部。例如，标签位于底部的QTabBar。
        checked,//该项目已选中。例如，QAbstractButton的选中状态。
        closable,//这些项目可以关闭。例如，QDockWidget打开了QDockWidgetDockWidgetClosable功能。
        default_,//该项目为默认值。例如，QMenu中的默认QPushButton或默认操作。
        disabled,//该项目已禁用。
        editable,//QComboBox是可编辑的。
        edit_focus,//该项具有编辑焦点(请参见QStyleState_HasEditFocus)。此状态仅适用于Qt扩展应用程序。
        enabled,//该项目已启用。
        exclusive,//该项目是独占项目组的一部分。例如，独占QActionGroup中的菜单项。
        first,//该项目是(列表中的)第一个项目。例如，QTabBar中的第一个选项卡。
        flat,//这件物品是平的。例如，平面QPushButton。
        floatable,//这些项目可以浮动。例如，QDockWidget打开了QDockWidget：：DockWidgetFloatable功能。
        focus,//该项具有输入焦点。
        has_children,//该项目具有子项。例如，QTreeView中具有子项的项。
        has_siblings,//该项目有同级。例如，QTreeView中的同级项。
        horizontal,//该项目具有水平方向
        hover,//鼠标悬停在该项目上。
        indeterminate,//该项具有不确定状态。例如，部分选中QCheckBox或QRadioButton。
        last,//该项是(列表中的)最后一项。例如，QTabBar中的最后一个选项卡。
        left,//该项目位于左侧。例如，选项卡位于左侧的QTabBar。
        maximized,//该项目将最大化。例如，最大化的QMdiSubWindow。
        middle,//项目在中间(在列表中)。例如，QTabBar中不在开头或结尾的制表符。
        minimized,//该项目被最小化。例如，最小化的QMdiSubWindow。
        movable,//物品可以四处移动。例如，QDockWidget打开了QDockWidgetDockWidgetMoovable功能。
        no_frame,//该项目没有框架。例如，无框架的QSpinBox或QLineEdit。
        non_exclusive,//该项是非独占项组的一部分。例如，非独占QActionGroup中的菜单项。
        off,//对于可以切换的项目，这适用于处于“关闭”状态的项目。
        on,//对于可以切换的项目，这适用于处于“打开”状态的widget。
        only_one,//该项目是(列表中的)唯一项目。例如，QTabBar中的一个单独的选项卡。
        open,//该项目处于打开状态。例如，QTreeView中的展开项，或具有打开菜单的QComboBox或QPushButton。
        next_selected,//选择下一项(在列表中)。例如，QTabBar的选定选项卡紧挨着该项。
        pressed,//正在使用鼠标按下该项。
        previous_selected,//选择上一项(在列表中)。例如，QTabBar中选定选项卡旁边的选项卡。
        read_only,//该项目标记为只读或不可编辑。例如，只读QLineEdit或不可编辑的QComboBox。
        right,//该项目位于右侧。例如，选项卡位于右侧的QTabBar。
        selected,//该项目即被选中。例如，QTabBar中的选定选项卡或QMenu中的选定项目。
        top,//该项目位于顶部。例如，选项卡位于顶部的QTabBar。
        unchecked,//该项目处于取消选中状态。
        vertical,//该项目具有垂直方向。
        window//widget是窗口(即顶层小部件)
    };
    StyleSheetBuilder();
    StyleSheetBuilder &setForeground(const QColor &color);
    StyleSheetBuilder &setBackground(const QColor &color);
    StyleSheetBuilder &setFontSize(const int &size);
    StyleSheetBuilder &setMargin(const int &left, const int &top, const int &right, const int &bottom);
    StyleSheetBuilder &setPadding(const int &left, const int &top, const int &right, const int &bottom);
    StyleSheetBuilder &setBorder(const int &left, const int &top, const int &right, const int &bottom);
    StyleSheetBuilder &setBorderRadius(const int &left, const int &top, const int &right, const int &bottom);
    StyleSheetBuilder &setHeight(const int &h);
    StyleSheetBuilder &setWidth(const int &w);
    StyleSheetBuilder &clean();
    QString toStyleSheet();
    QString toStyleSheet(QWidget *wdt, const QString &state = "");
    QString toStyleSheet(const QString &selectionName, const QString &state = "");
    StyleSheetBuilder &setBorder(const int &w, const QColor &color);
    QString toStateString(const StateTypes &st);
private:
    QJsonObject _mapOfSheet;
};
}
#endif // STYLESHEETBUILDER_H
