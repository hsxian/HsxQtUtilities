QT -= gui

TEMPLATE = lib
DEFINES += HSXQTUTILITIESCOREGIS_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GoesGeomHelper.cpp \
    TiffBox.cpp \
    TiffDemReader.cpp

HEADERS += \
    GoesGeomHelper.h \
    HsxQtUtilitiesGis_global.h \
    TiffBox.h \
    TiffDemReader.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/dependencies/geos/build/lib/ -lgeos

INCLUDEPATH += $$PWD/dependencies/geos/include \
            += $$PWD/dependencies/geos/build/include
DEPENDPATH += $$PWD/dependencies/geos/include
