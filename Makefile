all: libCommunication libLogConfigTime libVideoInput
 
libCommunication:
	make -C libCommunication/
	
libLogConfigTime:
	make -C libLogConfigTime/
	
libVideoInput:
	make -C libVideoInput/
	
clean:
	make -C libCommunication/ clean
	make -C libLogConfigTime/ clean
	make -C libVideoInput/ clean
	
.PHONY: all libCommunication libLogConfigTime libVideoInput clean