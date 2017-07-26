#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T02:20:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuickIndex
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    ndvi.cpp \
    ndwi.cpp \
    savi.cpp \
    gosavi.cpp \
    ndgi.cpp \
    ndbi.cpp \
    nbr.cpp \
    gdvi.cpp \
    evi2.cpp \
    gndvi.cpp \
    cigreen.cpp \
    ndsi.cpp \
    tvi.cpp \
    evi.cpp \
    grvi.cpp \
    lai.cpp \
    gvmi.cpp \
    gli.cpp \
    logratio.cpp \
    panndvi.cpp

HEADERS  += mainwindow.h \
    ndvi.h \
    ndwi.h \
    savi.h \
    gosavi.h \
    ndgi.h \
    ndbi.h \
    nbr.h \
    gdvi.h \
    evi2.h \
    gndvi.h \
    cigreen.h \
    ndsi.h \
    tvi.h \
    evi.h \
    grvi.h \
    lai.h \
    gvmi.h \
    gli.h \
    logratio.h \
    panndvi.h

FORMS    += mainwindow.ui \
    ndvi.ui \
    ndwi.ui \
    savi.ui \
    gosavi.ui \
    ndgi.ui \
    ndbi.ui \
    nbr.ui \
    gdvi.ui \
    evi2.ui \
    gndvi.ui \
    cigreen.ui \
    ndsi.ui \
    tvi.ui \
    evi.ui \
    grvi.ui \
    lai.ui \
    gvmi.ui \
    gli.ui \
    logratio.ui \
    panndvi.ui

win32: LIBS += -L$$PWD/'../../../../../Program Files/Cpp_Libs/gdal_64/lib/' -lgdal_i

INCLUDEPATH += $$PWD/'../../../../../Program Files/Cpp_Libs/gdal_64/include'
DEPENDPATH += $$PWD/'../../../../../Program Files/Cpp_Libs/gdal_64/include'

RESOURCES +=
