CC = g++
AR = ar

OPENCV_CFLAGS = `pkg-config --cflags opencv`
OPENCV_LIBS = `pkg-config --libs opencv`
IN_CXXFLAGS = $(CXXFLAGS) $(OPENCV_CFLAGS) -c -Wall -I include/ -I ../../Tracking/lib3dWorld/include -I ../libLogConfigTime/include/ -I ../libVideoInput/include/ -I ../libPlatformSpecifics/include/ -I ../libCommunication/include
IN_LDFLAGS = $(LDFLAGS) $(OPENCV_LDLAGS)

all: libCameraAbstraction.a

LIBCAMERAABSTRACTION_OBJECTS =  src/CameraLocalProxy.o src/CameraProxy.o src/CameraRemoteProxy.o
libCameraAbstraction.a: $(LIBCAMERAABSTRACTION_OBJECTS)
	$(AR) rcs libCommunication.a $(LIBCAMERAABSTRACTION_OBJECTS)

.cpp.o:
	$(CC) $(IN_CXXFLAGS) $< -o $@
	
clean:
	rm -rf src/*.o libCameraAbstraction.a
	
.PHONY: all clean