#-------------------------------------------------
#
# Project created by QtCreator 2015-08-16T22:15:05
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = App
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    vertex.cpp \
    connectionway.cpp \
    myscene.cpp \
    enum.cpp \
    weigthstable.cpp \
    customview.cpp \
    graphaplorithms.cpp \
    graphwindow.cpp \
    serializatorxml.cpp \
    dirmodelsave.cpp \
    dirmodelread.cpp \
    graphwindowbellmanford.cpp \
    adjacencymatrixtable.cpp \
    tspalgorithm.cpp \
    graphwindowtsp.cpp \
    euleralgorithm.cpp \
    grapheulerwindow.cpp

HEADERS  += mainwindow.h \
    vertex.h \
    connectionway.h \
    myscene.h \
    enum.h \
    weigthstable.h \
    customview.h \
    graphaplorithms.h \
    graphwindow.h \
    serializatorxml.h \
    dirmodelsave.h \
    dirmodelread.h \
    graphwindowbellmanford.h \
    adjacencymatrixtable.h \
    tspalgorithm.h \
    graphwindowtsp.h \
    euleralgorithm.h \
    grapheulerwindow.h

FORMS    += mainwindow.ui \
    weigthstable.ui \
    graphwindow.ui \
    dirmodelsave.ui \
    dirmodelread.ui \
    graphwindowbellmanford.ui \
    adjacencymatrixtable.ui \
    graphwindowtsp.ui \
    grapheulerwindow.ui
