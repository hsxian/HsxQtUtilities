QT += testlib network widgets
QT -= gui

CONFIG += c++11 qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
#DEPENDENCIES_PATH=$$PWD/../dependencies
DEPENDENCIES_PATH=$$PWD/../..

SOURCES += \ 
    main.cpp \
    SlideWinowCacheTest.cpp \
    OpencvHelperTest.cpp

INCLUDEPATH += $$PWD/../src

win32:CONFIG(release, debug|release):{
    LIBS +=  -L$$PWD/../release/ -lHsxQtUtilities
    system(resolve-win-resources.bat release $$DEPENDENCIES_PATH)
}
else:win32:CONFIG(debug, debug|release):{
    LIBS +=  -L$$PWD/../debug/ -lHsxQtUtilities
    system($$PWD/../resolve-win-resources.bat debug $$DEPENDENCIES_PATH)
}
else:unix:{
    LIBS += -L$$PWD/.. -lHsxQtUtilities
}

HEADERS += \
    OpencvHelperTest.h \
    SlideWinowCacheTest.h
