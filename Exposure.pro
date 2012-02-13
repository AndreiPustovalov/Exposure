#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T15:54:47
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=gnu++0x

FALSE {
INCLUDEPATH += "E:/My Documents/Programming/OpenCV-MinGW/opencv/include"
INCLUDEPATH += "E:/My Documents/Programming/OpenCV-MinGW/opencv/modules/core/include"
INCLUDEPATH += "E:/My Documents/Programming/OpenCV-MinGW/opencv/modules/highgui/include"

LIBS += "E:/My Documents/Programming/OpenCV-MinGW/mingw-build/lib/libopencv_core230.dll.a"
LIBS += "E:/My Documents/Programming/OpenCV-MinGW/mingw-build/lib/libopencv_highgui230.dll.a"
}

include(libs.pro)

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




