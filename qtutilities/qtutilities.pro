#-------------------------------------------------
#
# Project created by QtCreator 2022-07-15T09:22:23
#
#-------------------------------------------------

QT       += widgets network

QT       -= gui

TARGET = qtutilities
TEMPLATE = lib

DEFINES += QTUTILITIES_LIBRARY

SOURCES += thread/ThreadHelper.cpp \
    cache/FileCache.cpp \
    extension/StringEx.cpp \
    extension/TimeSpan.cpp \
    geometry/LineStegment.cpp \
    git/CommitInfo.cpp \
    git/GitHelper.cpp \
    git/GitTransferProgress.cpp \
    git/GitTypes.cpp \
    git/OId.cpp \
    humanization/HumanizationBytes.cpp \
    io/IoHelper.cpp \
    linq/CollectionHelper.cpp \
    math/UnitConversion.cpp \
    math/Variancer.cpp \
    ui/FlowLayout.cpp \
    ui/QStandardItemModelHelper.cpp \
    ui/StyleSheetBuilder.cpp \
    ui/WindowMoveHelper.cpp \
    ui/widgets/MuliSelCombox.cpp \
    ui/widgets/QRoundProgressBar.cpp

HEADERS += cache/SlideWindowCache.h \
        cache/FileCache.h \
        common/CommonDefine.h \
        extension/StringEx.h \
        extension/TimeSpan.h \
        geometry/LineStegment.h \
        git/CommitInfo.h \
        git/GitHelper.h \
        git/GitTransferProgress.h \
        git/GitTypes.h \
        git/OId.h \
        humanization/HumanizationBytes.h \
        io/IoHelper.h \
        linq/CollectionHelper.h \
        math/UnitConversion.h \
        math/Variancer.h \
        qtutilities_global.h \
        thread/ThreadHelper.h \
        ui/FlowLayout.h \
        ui/QStandardItemModelHelper.h \
        ui/StyleSheetBuilder.h \
        ui/WindowMoveHelper.h \
        ui/widgets/MuliSelCombox.h \
        ui/widgets/QRoundProgressBar.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../dependencies/libgit2/include \




