QT -= gui
QT += widgets
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
    ui/FlowLayout.cpp \
    ui/QStandardItemModelHelper.cpp \
    ui/StyleSheetBuilder.cpp \
    ui/StyleSheetParser.cpp \
    ui/WindowMoveHelper.cpp \
    ui/widgets/MuliSelCombox.cpp \
    ui/widgets/QRoundProgressBar.cpp

HEADERS += \
    HsxQtUtilitiesCore_global.h \
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
    thread/ThreadHelper.h \
    ui/FlowLayout.h \
    ui/QStandardItemModelHelper.h \
    ui/StyleSheetBuilder.h \
    ui/StyleSheetParser.h \
    ui/WindowMoveHelper.h \
    ui/widgets/MuliSelCombox.h \
    ui/widgets/QRoundProgressBar.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
