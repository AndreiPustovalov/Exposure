#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T15:54:47
#
#-------------------------------------------------

QT       += core gui

#QMAKE_CXXFLAGS += -std=gnu++0x

include(libs.pro)
#include(libsvc.pro)

TARGET = Exposure
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videoprocessor.cpp

HEADERS  += mainwindow.h \
    videoprocessor.h \
    CvWindow.hpp

FORMS    += mainwindow.ui

RESOURCES += \
    Icons.qrc

OTHER_FILES += \
    icons/video-file.png \
    icons/flip-vertical.png \
    icons/flip-horizontal.png \
    icons/faq.png \
    icons/eraser.png




