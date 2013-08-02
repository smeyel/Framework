CC = g++
AR = ar
OPENCV_CFLAGS = `pkg-config --cflags opencv`
OPENCV_LDFLAGS = `pkg-config --libs opencv`
CFLAGS = -c -Wall -I include/ -I ../libLogConfigTime/include/ $(OPENCV_CFLAGS)
LDFLAGS =  $(OPENCV_LDFLAGS)

all: libVideoInput.a

LIBVIDEOINPUT_OBJECTS =  src/VideoInputFactory.o src/VideoInputV4L2.o src/VideoInputGeneric.o src/VideoInputPs3Eye.o
TESTVIDEOINPUT_OBJECTS = test/testVideoInput.o

libVideoInput.a: $(LIBVIDEOINPUT_OBJECTS)
	$(AR) rcs libVideoInput.a $(LIBVIDEOINPUT_OBJECTS)

testVideoInput: $(TESTVIDEOINPUT_OBJECTS) libVideoInput.a
		$(CC) -o testVideoInput $(TESTVIDEOINPUT_OBJECTS) libVideoInput.a $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf src/*.o test/*.o libVideoInput.a testVideoInput
	
.PHONY: all clean
