CC=g++
INSTALL_DIR=/usr/local/bin

all: build cmd
	
build:
	mkdir build
	
cmd: 
	$(CC) *.cpp -o build/diskco
	chmod +x build/diskco
	install build/diskco $(INSTALL_DIR)
	
clean:
	rm -rf build
