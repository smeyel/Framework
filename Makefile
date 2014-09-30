all: libLogConfigTime libPlatformSpecifics libCommunication
 
libLogConfigTime:
	make -C libLogConfigTime/

libPlatformSpecifics:
	make -C libPlatformSpecifics/

libCommunication:
	make -C libCommunication/
	
	
libVideoInput:
	make -C libVideoInput/
	
clean:
	make -C libCommunication/ clean
	make -C libLogConfigTime/ clean
	make -C libVideoInput/ clean
	make -C libPlatformSpecifics/ clean
	
.PHONY: all libCommunication libPlatformSpecifics libLogConfigTime libVideoInput clean
