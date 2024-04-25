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
BUILD_WITH_KAFKA \
BUILD_WITH_RABBITMQ \
BUILD_WITH_OPENCV \

#defineReplace(remove_extra_config_parameter) {
#    configs = $$1
#    debug_and_release_params = # 匹配预选队�#    keys = debug Debug release Release debug_and_release
#    for (iter, configs) {
#        contains(keys, $$iter) {
#            debug_and_release_params += $$iter
#        }
#    }

#    for (iter, debug_and_release_params) {
#        configs -= $$iter # 移除预选队列的属�#    }

#    configs += $$last(debug_and_release_params) # 添加(保留)预选队列的最后属�
#    return($$configs)
#}

## 使用
#CONFIG = $$remove_extra_config_parameter($$CONFIG)

#DEPENDENCIES_PATH=$$PWD/dependencies
DEPENDENCIES_PATH=$$PWD/..

CONFIG(debug,debug|release):{
    BUILD_TYPE=debug
}
else:CONFIG(release,debug|release):{
    BUILD_TYPE=release
}
win32:{
    system(resolve-win-resources.bat debug $$DEPENDENCIES_PATH)
}


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
    src/ui/widgets/QRoundProgressBar.cpp \
    src/extension/RegularHelper.cpp \

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
    src/ui/widgets/QRoundProgressBar.h \
    src/extension/RegularHelper.h \


INCLUDEPATH += src/

contains(DEFINES,BUILD_WITH_GIT){

    INCLUDEPATH += $$DEPENDENCIES_PATH/libgit2/include

    win32: LIBS +=  -L$$DEPENDENCIES_PATH/libgit2/build/$$BUILD_TYPE/ -lgit2
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

    win32: LIBS +=  -L$$DEPENDENCIES_PATH/geos/build/lib/$$BUILD_TYPE/ -lgeos
    else:unix: LIBS += -L$$DEPENDENCIES_PATH/geos/build/lib/ -lgeos

    SOURCES += \
        src/gis/GoesGeomHelper.cpp

    HEADERS += \
        src/gis/GoesGeomHelper.h
}

contains(DEFINES,BUILD_WITH_KAFKA){

    INCLUDEPATH += $$DEPENDENCIES_PATH/librdkafka/src-cpp

    win32:{
        LIBS +=  -L$$DEPENDENCIES_PATH/librdkafka/build/src/$$BUILD_TYPE/ -lrdkafka \
                 -L$$DEPENDENCIES_PATH/librdkafka/build/src-cpp/$$BUILD_TYPE/ -lrdkafka++
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


contains(DEFINES,BUILD_WITH_RABBITMQ){

    INCLUDEPATH += $$DEPENDENCIES_PATH/rabbitmq-c/include \
                += $$DEPENDENCIES_PATH/rabbitmq-c/build/include

    win32:{
        LIBS +=  -L$$DEPENDENCIES_PATH/rabbitmq-c/build/librabbitmq/$$BUILD_TYPE/ -lrabbitmq.4
    }
    else:unix:{
        LIBS += -L$$DEPENDENCIES_PATH/rabbitmq-c/build/librabbitmq -lrabbitmq.4
    }

    SOURCES += \
        src/mq/RabbitMQ.cpp
    HEADERS += \
        src/mq/RabbitMQ.h
}

contains(DEFINES,BUILD_WITH_OPENCV){

    INCLUDEPATH += $$DEPENDENCIES_PATH/opencv/include \
                += $$DEPENDENCIES_PATH/opencv/build \
                += $$DEPENDENCIES_PATH/opencv/build/include/opencv2/core/hal \
                += $$DEPENDENCIES_PATH/opencv/modules/core/include \
                += $$DEPENDENCIES_PATH/opencv/modules/calib3d/include \
                += $$DEPENDENCIES_PATH/opencv/modules/features2d/include \
                += $$DEPENDENCIES_PATH/opencv/modules/flann/include \
                += $$DEPENDENCIES_PATH/opencv/modules/dnn/include \
                += $$DEPENDENCIES_PATH/opencv/modules/highgui/include \
                += $$DEPENDENCIES_PATH/opencv/modules/imgcodecs/include \
                += $$DEPENDENCIES_PATH/opencv/modules/videoio/include \
                += $$DEPENDENCIES_PATH/opencv/modules/imgproc/include \
                += $$DEPENDENCIES_PATH/opencv/modules/ml/include \
                += $$DEPENDENCIES_PATH/opencv/modules/objdetect/include \
                += $$DEPENDENCIES_PATH/opencv/modules/photo/include \
                += $$DEPENDENCIES_PATH/opencv/modules/shape/include \
                += $$DEPENDENCIES_PATH/opencv/modules/stitching/include \
                += $$DEPENDENCIES_PATH/opencv/modules/superres/include \
                += $$DEPENDENCIES_PATH/opencv/modules/video/include \
                += $$DEPENDENCIES_PATH/opencv/modules/videostab/include \

    win32:CONFIG(debug,debug|release):{
        LIBS +=  -L$$DEPENDENCIES_PATH/opencv/build/lib/$$BUILD_TYPE/ -lopencv_core342d -lopencv_videoio342d -lopencv_imgcodecs342d
    }
    else:win32:CONFIG(release,debug|release):{
        LIBS +=  -L$$DEPENDENCIES_PATH/opencv/build/lib/$$BUILD_TYPE/ -lopencv_core342 -lopencv_videoio342 -lopencv_imgcodecs342
    }
    else:unix:{
        LIBS += -L$$DEPENDENCIES_PATH/opencv/build/lib -lopencv_core342 -lopencv_videoio342 -lopencv_imgcodecs342
    }

    SOURCES += \
        src/media/OpencvHelper.cpp
    HEADERS += \
        src/media/OpencvHelper.h
}

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
