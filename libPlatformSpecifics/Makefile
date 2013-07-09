CC = g++
AR = ar
CFLAGS = -c -Wall -I include/ -I ../libLogConfigTime/include/
LDFLAGS = 

all: libPlatformSpecifics.a

LIBPLATFORMSPECIFICS_OBJECTS =  src/PlatformSpecifics.o src/PlatformSpecificsLinux.o src/PlatformSpecificsWin32.o


libPlatformSpecifics.a: $(LIBPLATFORMSPECIFICS_OBJECTS)
	$(AR) rcs libPlatformSpecifics.a $(LIBPLATFORMSPECIFICS_OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf src/*.o libPlatformSpecifics.a
	
.PHONY: all clean
