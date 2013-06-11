CC = gcc
AR = ar
CFLAGS = -c -Wall -I include/
LDFLAGS = 

all: libLogConfigTime.a


LIBLOGCONFIGTIME_OBJECTS = src/ConfigReader.o src/ConfigReader.o src/ConvertUTF.c src/FileLogger.o src/FileLogger.o src/Logger.o src/Logger.o src/SimpleIniConfigReader.o src/StdOutLogger.o src/TimeMeasurement.o


libLogConfigTime.a: $(LIBLOGCONFIGTIME_OBJECTS)
	$(AR) rcs libLogConfigTime.a $(LIBLOGCONFIGTIME_OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
.c.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf src/*.o libLogConfigTime.a
	
.PHONY: all clean