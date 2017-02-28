######################################################################
# Automatically generated by qmake (2.01a) ?? ??? 4 13:41:56 2016
######################################################################

TEMPLATE = app
TARGET = atlas
DEPENDPATH += .
INCLUDEPATH += .
QT += sql
QT += testlib

# Input
INCLUDEPATH += ./src
INCLUDEPATH += ./include
INCLUDEPATH += ./languages
INCLUDEPATH += ./ui

HEADERS += include/bookmarktablemodel.h \
           include/bookmarkview.h \
           include/bookmarkwidget.h \
           include/glossarytablemodel.h \
           include/glossaryview.h \
           include/glossarywidget.h \
           include/historytablemodel.h \
           include/historyview.h \
           include/historywidget.h \
           include/mainwindow.h \
           include/notepadwidget.h \
           include/notetablemodel.h \
           include/notewidget.h \
           include/startwindow.h \
           include/usertablemodel.h \
           include/userview.h \
           include/userwidget.h \
    include/integratortablemodel.h \
    include/integratetableview.h \
    include/integratewidget.h \
    include/simulatortablemodel.h \
    include/simulatortableview.h \
    include/simulatorwidget.h \
    include/glossary.h \
    include/simulatorstarter.h \
    include/noteview.h
FORMS += ui/mainwindow.ui
SOURCES += src/bookmarktablemodel.cpp \
           src/bookmarkview.cpp \
           src/bookmarkwidget.cpp \
           src/glossarytablemodel.cpp \
           src/glossaryview.cpp \
           src/glossarywidget.cpp \
           src/historytablemodel.cpp \
           src/historyview.cpp \
           src/historywidget.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/notepadwidget.cpp \
           src/notetablemodel.cpp \
           src/notewidget.cpp \
           src/startwindow.cpp \
           src/usertablemodel.cpp \
           src/userview.cpp \
           src/userwidget.cpp \
    src/integratetableview.cpp \
    src/integratewidget.cpp \
    src/simulatortablemodel.cpp \
    src/simulatorwidget.cpp \
    src/simulatortableview.cpp \
    src/glossary.cpp \
    src/simulatorstarter.cpp \
    src/noteview.cpp \
    src/integratetablemodel.cpp
TRANSLATIONS += languges/main_ru.ts \
                languges/main_en.ts
