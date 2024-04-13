QT -= gui
QT += widgets network
TEMPLATE = lib
DEFINES += HSXQTUTILITIESCORE_LIBRARY

CONFIG += c++11
MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithm/HsxKMeans.cpp \
    algorithm/Kalman.cpp \
    cache/AsyncMapCache.cpp \
    system/HsxNativeEvent.cpp \
    thread/ThreadHelper.cpp \
    cache/FileCache.cpp \
    extension/StringEx.cpp \
    extension/TimeSpan.cpp \
    geometry/LineStegment.cpp \
    humanization/HumanizationBytes.cpp \
    io/IoHelper.cpp \
    linq/CollectionHelper.cpp \
    math/UnitConversion.cpp \
    math/Variancer.cpp \
    ui/ExclusiveWindow.cpp \
    ui/FlowLayout.cpp \
    ui/GradientStyleBuilder.cpp \
    ui/GridLayoutHelper.cpp \
    ui/ImageHelper.cpp \
    ui/LayoutHelper.cpp \
    ui/ModelUpdateToViewHelp.cpp \
    ui/StyleSheetBuilder.cpp \
    ui/StyleSheetParser.cpp \
    ui/WindowMoveHelper.cpp \
    ui/widgets/ImageWidget.cpp \
    ui/widgets/MuliSelCombox.cpp \
    ui/widgets/PaintEventWidget.cpp \
    ui/widgets/QRoundProgressBar.cpp

HEADERS += \
    HsxQtUtilitiesCore_global.h \
    algorithm/HsxKMeans.h \
    algorithm/Kalman.h \
    cache/AsyncMapCache.h \
    cache/SlideWindowCache.h \
    cache/FileCache.h \
    common/CommonDefine.h \
    extension/StringEx.h \
    extension/TimeSpan.h \
    geometry/LineStegment.h \
    humanization/HumanizationBytes.h \
    io/IoHelper.h \
    linq/CollectionHelper.h \
    math/UnitConversion.h \
    math/Variancer.h \
    system/HsxNativeEvent.h \
    thread/ThreadHelper.h \
    ui/CommonSortFilterProxyModel.h \
    ui/ExclusiveWindow.h \
    ui/FlowLayout.h \
    ui/GradientStyleBuilder.h \
    ui/GridLayoutHelper.h \
    ui/ImageHelper.h \
    ui/LayoutHelper.h \
    ui/ModelUpdateToViewHelp.h \
    ui/QStandardItemModelHelper.h \
    ui/StyleSheetBuilder.h \
    ui/StyleSheetParser.h \
    ui/WindowMoveHelper.h \
    ui/widgets/ImageWidget.h \
    ui/widgets/MuliSelCombox.h \
    ui/widgets/PaintEventWidget.h \
    ui/widgets/QRoundProgressBar.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

