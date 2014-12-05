CC = g++
AR = ar
CFLAGS = -c -Wall -I include/ -I ../libLogConfigTime/include/
LDFLAGS = 

all: libVideoInput.a

LIBVIDEOINPUT_OBJECTS =  src/VideoInputFactory.o src/VideoInputGeneric.o 
#src/VideoInputPs3Eye.o


libVideoInput.a: $(LIBVIDEOINPUT_OBJECTS)
	$(AR) rcs libVideoInput.a $(LIBVIDEOINPUT_OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf src/*.o libVideoInput.a
	
.PHONY: all clean