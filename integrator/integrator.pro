######################################################################
# Automatically generated by qmake (2.01a) ?? ??? 4 13:41:56 2016
######################################################################

TEMPLATE = app
TARGET = atlas
DEPENDPATH += .
INCLUDEPATH += .
QT += sql widgets
QT += testlib

# Input
INCLUDEPATH += ./src
INCLUDEPATH += ./include
INCLUDEPATH += ./languages
INCLUDEPATH += ./ui
QMAKE_CXX_FLAGS += -std=c++11

HEADERS += include/mainwindow.h \
           include/startwindow.h \
           include/usertablemodel.h \
           include/userview.h \
           include/userwidget.h \
    include/integratortablemodel.h \
    include/integratetableview.h \
    include/integratewidget.h \
    include/simulatorstarter.h \
    include/filetotabwidget.h \
    include/printelement.h \
    include/shell.h

FORMS += ui/mainwindow.ui

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/startwindow.cpp \
           src/usertablemodel.cpp \
           src/userview.cpp \
           src/userwidget.cpp \
    src/integratetableview.cpp \
    src/integratewidget.cpp \
    src/simulatorstarter.cpp \
    src/integratetablemodel.cpp \
    src/filetotabwidget.cpp \
    src/printelement.cpp \
    src/shell.cpp \

TRANSLATIONS += languges/main_ru.ts \
                languges/main_en.ts