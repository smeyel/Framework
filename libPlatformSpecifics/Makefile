CC = g++
AR = ar
CFLAGS = -c -Wall -I include/ -I ../libLogConfigTime/include/
LDFLAGS = 

all: libPlatformSpecifics.a

LIBCOMMUNICATION_OBJECTS =  src/PlatformSpecifics.o src/PlatformSpecificsLinux.o src/PlatformSpecificsWin32.o


libPlatformSpecifics.a: $(LIBCOMMUNICATION_OBJECTS)
	$(AR) rcs libPlatformSpecifics.a $(LIBCOMMUNICATION_OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf src/*.o libPlatformSpecifics.a
	
.PHONY: all clean