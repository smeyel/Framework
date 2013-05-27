CC = g++
AR = ar
CFLAGS = -c -Wall -I include/ -I ../libLogConfigTime/include/
LDFLAGS = 

all: libCommunication.a
#libMiscTimeAndConfig libVideoInput libLogConfigTime libMiscTimeAndConfigAndroid

LIBCOMMUNICATION_OBJECTS =  src/JpegMessage.o src/JsonMessage.o src/JsonWithAuxMessage.o src/MatImageMessage.o src/MeasurementLogMessage.o src/PingMessage.o src/SendlogMessage.o src/SendPositionMessage.o src/TakePictureMessage.o
#src/PhoneProxy.o
#src/PhoneServer.o


libCommunication.a: $(LIBCOMMUNICATION_OBJECTS)
	$(AR) rcs libCommunication.a $(LIBCOMMUNICATION_OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@