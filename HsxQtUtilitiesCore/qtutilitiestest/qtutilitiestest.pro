QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_slidewinowcachetest.cpp

INCLUDEPATH += $$PWD/../qtutilities
unix|win32: LIBS += -L$$PWD/../qtutilities/ -lqtutilities
