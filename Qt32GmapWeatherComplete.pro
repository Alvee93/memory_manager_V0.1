#-------------------------------------------------
#
# Project created by QtCreator 2017-08-13T15:42:37
#
#-------------------------------------------------

QT       += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt32GmapWeatherComplete
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    exif.cpp \
    gmapclass.cpp \
    weatherclass.cpp \
    daylightclass.cpp

HEADERS  += mainwindow.h \
    exif.h \
    gmapclass.h \
    weatherclass.h \
    daylightclass.h

FORMS    += mainwindow.ui
