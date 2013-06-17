all: libCommunication libLogConfigTime libVideoInput libPlatformSpecifics libCameraAbstraction
 
libCommunication:
	make -C libCommunication/
	
libLogConfigTime:
	make -C libLogConfigTime/
	
libVideoInput:
	make -C libVideoInput/
	
libCameraAbstraction:
	make -C libCameraAbstraction/
	
libPlatformSpecifics:
	make -C libPlatformSpecifics/
	
clean:
	make -C libCommunication/ clean
	make -C libLogConfigTime/ clean
	make -C libVideoInput/ clean
	make -C libCameraAbstraction/ clean
	make -C libPlatformSpecifics/ clean
	
.PHONY: all libCommunication libLogConfigTime libVideoInput libCameraAbstraction libPlatformSpecifics clean