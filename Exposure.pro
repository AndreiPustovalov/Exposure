#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T15:54:47
#
#-------------------------------------------------

QT       += core gui

INCLUDEPATH += "D:/OpenCV2.3/opencv/include"
INCLUDEPATH += "D:/OpenCV2.3/opencv/modules/core/include"
INCLUDEPATH += "D:/OpenCV2.3/opencv/modules/highgui/include"

LIBS += "D:/OpenCV2.3/mingw-build/lib/libopencv_core230.dll.a"
LIBS += "D:/OpenCV2.3/mingw-build/lib/libopencv_highgui230.dll.a"

TARGET = Exposure
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videoprocessor.cpp

HEADERS  += mainwindow.h \
    videoprocessor.h

FORMS    += mainwindow.ui


