QT -= gui

TEMPLATE = lib
DEFINES += HSXQTUTILITIESGIT_LIBRARY

CONFIG += c++11
MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CommitInfo.cpp \
    GitHelper.cpp \
    GitTransferProgress.cpp \
    GitTypes.cpp \
    OId.cpp

HEADERS += \
    HsxQtUtilitiesGit_global.h \
    CommitInfo.h \
    GitHelper.h \
    GitTransferProgress.h \
    GitTypes.h \
    OId.h


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/dependencies/libgit2/build/ -lgit2

INCLUDEPATH += $$PWD/dependencies/libgit2/include
DEPENDPATH += $$PWD/dependencies/libgit2/include
