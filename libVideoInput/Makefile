CC = g++
AR = ar
CFLAGS = -c -Wall -I include/ -I ../libLogConfigTime/include/
LDFLAGS = 

all: libVideoInput.a
#libMiscTimeAndConfig libVideoInput libLogConfigTime libMiscTimeAndConfigAndroid

LIBVIDEOINPUT_OBJECTS =  src/VideoInputFactory.o src/VideoInputGeneric.o 
#src/VideoInputPs3Eye.o


libVideoInput.a: $(LIBVIDEOINPUT_OBJECTS)
	$(AR) rcs libLogConfigTime.a $(LIBVIDEOINPUT_OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@