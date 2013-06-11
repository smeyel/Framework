CC = g++
AR = ar
IN_CXXFLAGS = $(CXXFLAGS) -c -Wall -I include/ -I ../libLogConfigTime/include/ -I ../libPlatformSpecifics/include/ -I /lib
IN_LDFLAGS = $(LDFLAGS)

all: libCommunication.a
#libMiscTimeAndConfig libVideoInput libLogConfigTime libMiscTimeAndConfigAndroid

LIBCOMMUNICATION_OBJECTS =  src/JpegMessage.o src/JsonMessage.o src/JsonWithAuxMessage.o src/MatImageMessage.o src/MeasurementLogMessage.o src/PingMessage.o src/SendlogMessage.o src/SendPositionMessage.o src/TakePictureMessage.o src/TextMessage.o src/PhoneProxy.o src/PhoneServer.o


libCommunication.a: $(LIBCOMMUNICATION_OBJECTS)
	$(AR) rcs libCommunication.a $(LIBCOMMUNICATION_OBJECTS)

.cpp.o:
	$(CC) $(IN_CXXFLAGS) $< -o $@
	
clean:
	rm -rf src/*.o libCommunication.a
	
.PHONY: all clean