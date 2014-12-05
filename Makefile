all: libLogConfigTime libPlatformSpecifics libCommunication libVideoInput libCameraAbstraction
 
libLogConfigTime:
	make -C libLogConfigTime/

libPlatformSpecifics:
	make -C libPlatformSpecifics/

libCommunication:
	make -C libCommunication/
	
	
libCameraAbstraction:
	make -C libCameraAbstraction/
	
libVideoInput:
	make -C libVideoInput/
	
clean:
	make -C libCommunication/ clean
	make -C libLogConfigTime/ clean
	make -C libVideoInput/ clean
	make -C libPlatformSpecifics/ clean
	make -C libCameraAbstraction/ clean
	
.PHONY: all libCommunication libPlatformSpecifics libLogConfigTime libVideoInput libCameraAbstraction clean
