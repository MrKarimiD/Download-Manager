#-------------------------------------------------
#
# Project created by QtCreator 2014-06-22T15:07:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DownloadManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    downloadmanager.cpp \
    downloadfragmnet.cpp

HEADERS  += mainwindow.h \
    downloadmanager.h \
    downloadfragmnet.h

FORMS    += mainwindow.ui
