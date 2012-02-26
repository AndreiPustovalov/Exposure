#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T15:54:47
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG   += thread

INCLUDEPATH += "E:/Libs/opencv/include"
INCLUDEPATH += "E:/Libs/opencv/modules/core/include"
INCLUDEPATH += "E:/Libs/opencv/modules/highgui/include"

declarative_debug {
    message("debug_build")
    LIBS += "E:/Libs/opencv/build/lib/Debug/opencv_core233d.lib"
    LIBS += "E:/Libs/opencv/build/lib/Debug/opencv_highgui233d.lib"
}

!declarative_debug {
    message("release_build")
    LIBS += "E:/Libs/opencv/build/lib/Release/opencv_core233.lib"
    LIBS += "E:/Libs/opencv/build/lib/Release/opencv_highgui233.lib"
}

TARGET = Exposure
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videoprocessor.cpp \
    calc.cpp \
    cvwindow.cpp

HEADERS  += mainwindow.h \
    videoprocessor.h \
    operations.hpp \
    calc.h \
    cvwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    Icons.qrc

OTHER_FILES += \
    icons/video-file.png \
    icons/flip-vertical.png \
    icons/flip-horizontal.png \
    icons/faq.png \
    icons/eraser.png \
    icons/full-screen.png








