#TEMPLATE = app
#CONFIG += console c++11
#CONFIG -= app_bundle
#CONFIG -= qt

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = object_detect
TEMPLATE = app

SOURCES += main.cpp \
    objectdetect.cpp \
    mainwindow.cpp \
    informationwindow.cpp \
    imagelable.cpp

INCLUDEPATH +=   /usr/local/opencv-2.4.10/include\
                          /usr/local/opencv-2.4.10/include/opencv\
                          /usr/local/opencv-2.4.10/include/opencv2\


LIBS += -L/usr/local/opencv-2.4.10/lib -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_objdetect

HEADERS += \
    objectdetect.h \
    mainwindow.h \
    informationwindow.h \
    imagelable.h

FORMS += \
    mainwindow.ui \
    informationwindow.ui
