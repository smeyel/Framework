CC = g++
AR = ar
CFLAGS = -c -Wall -Wno-unused-variable \
	-I include/ \
	-I ../libCommunication/include/ \
	-I ../libLogConfigTime/include/ \
	-I ../libPlatformSpecifics/include/ \
	-I ../libVideoInput/include/ \
	-I ../../../Tracking/lib3dWorld/include/ \
	-I /home/akos/Programs/OpenCV/opencv-2.4.9/include
	
LDFLAGS = 

all: libCameraAbstraction.a

SRC = 		$(wildcard src/*.cpp)
OBJS =		$(addprefix obj/,$(notdir $(SRC:.cpp=.o))) 


libCameraAbstraction.a: $(OBJS)
	$(AR) rcs libCameraAbstraction.a $(OBJS)

obj/%.o: src/%.cpp objdir
	$(CC) $(CFLAGS) -o $@ $<
#.cpp.o:
#	$(CC) $(CFLAGS) $< -o $@

objdir:
	mkdir -p obj
	
clean:
	rm -rf $(OBJS) libCameraAbstraction.a
	
.PHONY: all clean
