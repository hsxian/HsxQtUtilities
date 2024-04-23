QT -= gui
QT += widgets network sql
TEMPLATE = lib
DEFINES += HSXQTUTILITIES_LIBRARY
CONFIG += c++11

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj

DEFINES += \
BUILD_WITH_GIT \
BUILD_WITH_GIS \
BUILD_WITH_KAFKA

#DEPENDENCIES_PATH=$$PWD/dependencies
DEPENDENCIES_PATH=$$PWD/..
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/algorithm/HsxKMeans.cpp \
    src/algorithm/Kalman.cpp \
    src/cache/AsyncMapCache.cpp \
    src/data/QSqliteHelper.cpp \
    src/system/HsxNativeEvent.cpp \
    src/thread/ThreadHelper.cpp \
    src/cache/FileCache.cpp \
    src/extension/StringEx.cpp \
    src/extension/TimeSpan.cpp \
    src/geometry/LineStegment.cpp \
    src/humanization/HumanizationBytes.cpp \
    src/io/IoHelper.cpp \
    src/linq/CollectionHelper.cpp \
    src/math/UnitConversion.cpp \
    src/math/Variancer.cpp \
    src/ui/ExclusiveWindow.cpp \
    src/ui/FlowLayout.cpp \
    src/ui/GradientStyleBuilder.cpp \
    src/ui/GridLayoutHelper.cpp \
    src/ui/ImageHelper.cpp \
    src/ui/LayoutHelper.cpp \
    src/ui/ModelUpdateToViewHelp.cpp \
    src/ui/StyleSheetBuilder.cpp \
    src/ui/StyleSheetParser.cpp \
    src/ui/WindowMoveHelper.cpp \
    src/ui/widgets/ImageWidget.cpp \
    src/ui/widgets/MuliSelCombox.cpp \
    src/ui/widgets/PaintEventWidget.cpp \
    src/ui/widgets/QRoundProgressBar.cpp

HEADERS += \
    src/HsxQtUtilities_global.h \
    src/algorithm/HsxKMeans.h \
    src/algorithm/Kalman.h \
    src/cache/AsyncMapCache.h \
    src/cache/SlideWindowCache.h \
    src/cache/FileCache.h \
    src/common/CommonDefine.h \
    src/data/QSqliteHelper.h \
    src/extension/StringEx.h \
    src/extension/TimeSpan.h \
    src/geometry/LineStegment.h \
    src/humanization/HumanizationBytes.h \
    src/io/IoHelper.h \
    src/linq/CollectionHelper.h \
    src/math/UnitConversion.h \
    src/math/Variancer.h \
    src/system/HsxNativeEvent.h \
    src/thread/ThreadHelper.h \
    src/ui/CommonSortFilterProxyModel.h \
    src/ui/ExclusiveWindow.h \
    src/ui/FlowLayout.h \
    src/ui/GradientStyleBuilder.h \
    src/ui/GridLayoutHelper.h \
    src/ui/ImageHelper.h \
    src/ui/LayoutHelper.h \
    src/ui/ModelUpdateToViewHelp.h \
    src/ui/StyleSheetBuilder.h \
    src/ui/StyleSheetParser.h \
    src/ui/WindowMoveHelper.h \
    src/ui/widgets/ImageWidget.h \
    src/ui/widgets/MuliSelCombox.h \
    src/ui/widgets/PaintEventWidget.h \
    src/ui/widgets/QRoundProgressBar.h

INCLUDEPATH += src/

contains(DEFINES,BUILD_WITH_GIT){

    INCLUDEPATH += $$DEPENDENCIES_PATH/libgit2/include

    win32:CONFIG(release, debug|release): LIBS +=  -L$$DEPENDENCIES_PATH/libgit2/release/ -lgit2
    else:win32:CONFIG(debug, debug|release): LIBS +=  -L$$DEPENDENCIES_PATH/libgit2/build/debug/ -lgit2
    else:unix: LIBS += -L$$DEPENDENCIES_PATH/libgit2/build/ -lgit2

    SOURCES += \
        src/git/CommitInfo.cpp \
        src/git/GitHelper.cpp \
        src/git/GitTransferProgress.cpp \
        src/git/GitTypes.cpp \
        src/git/OId.cpp

    HEADERS += \
        src/git/CommitInfo.h \
        src/git/GitHelper.h \
        src/git/GitTransferProgress.h \
        src/git/GitTypes.h \
        src/git/OId.h
}

contains(DEFINES,BUILD_WITH_GIS){

    INCLUDEPATH += $$DEPENDENCIES_PATH/geos/include \
                += $$DEPENDENCIES_PATH/geos/build/include \
                += $$DEPENDENCIES_PATH/gdal/gdal/gcore

    win32:CONFIG(release, debug|release): LIBS +=  -L$$DEPENDENCIES_PATH/geos/build/lib/release/ -lgeos
    else:win32:CONFIG(debug, debug|release): LIBS +=  -L$$DEPENDENCIES_PATH/geos/build/lib/debug/ -lgeos
    else:unix: LIBS += -L$$DEPENDENCIES_PATH/geos/build/lib/ -lgeos

    SOURCES += \
        src/gis/GoesGeomHelper.cpp

    HEADERS += \
        src/gis/GoesGeomHelper.h
}

contains(DEFINES,BUILD_WITH_KAFKA){

    INCLUDEPATH += $$DEPENDENCIES_PATH/librdkafka/src-cpp

    win32:CONFIG(release, debug|release):{
        LIBS +=  -L$$DEPENDENCIES_PATH/librdkafka/build/src/release/ -lrdkafka \
                 -L$$DEPENDENCIES_PATH/librdkafka/build/src-cpp/release/ -lrdkafka++
    }
    else:win32:CONFIG(debug, debug|release):{
        LIBS +=  -L$$DEPENDENCIES_PATH/librdkafka/build/src/debug/ -lrdkafka \
                 -L$$DEPENDENCIES_PATH/librdkafka/build/src-cpp/debug/ -lrdkafka++
    }
    else:unix:{
        LIBS += -L$$DEPENDENCIES_PATH/librdkafka/build/src -lrdkafka \
                -L$$DEPENDENCIES_PATH/librdkafka/build/src-cpp -lrdkafka++
    }


    SOURCES += \
        src/mq/KafkaClient.cpp
    HEADERS += \
        src/mq/KafkaClient.h
}

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
