MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = -pipe -O2 -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -std=c++0x -Wall -W -fPIC $(DEFINES)
#install path of OpenCV
OPENCVDIR     = /usr/local/opencv-2.4.10
LINKFLAGS     = -L$(OPENCVDIR)/lib -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_objdetect -Wl,-rpath=$(OPENCVDIR)/lib
OPENCVINC     = $(OPENCVDIR)/include
INCPATH       = -I$(OPENCVINC)


BUILD_DIR     = ./bin/
#bin
TARGET        = objectdetect

OBJECTS       = main.o \
                objectdetect.o 

all:$(OBJECTS)
	cd $(BUILD_DIR) && $(CXX) $^ $(LINKFLAGS) -o $(TARGET)

$(OBJECTS):%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(BUILD_DIR)$@ $<

.PHONY:clean

clean :
	cd $(BUILD_DIR) && rm $(TARGET) $(OBJECTS)
