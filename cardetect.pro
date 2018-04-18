TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    objectdetect.cpp

INCLUDEPATH +=   /usr/local/opencv-2.4.10/include\
                          /usr/local/opencv-2.4.10/include/opencv\
                          /usr/local/opencv-2.4.10/include/opencv2

LIBS += -L/usr/local/opencv-2.4.10/lib -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_objdetect

HEADERS += \
    objectdetect.h
