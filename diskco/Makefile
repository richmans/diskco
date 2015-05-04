CC=g++
INSTALL_DIR=/usr/local/bin
SOURCES=buffer.cpp \
				buffer_pool.cpp \
				buffer_processor.cpp \
				byte_swapper.cpp \
				diskco.cpp \
				file_reader.cpp \
				file_writer.cpp \
				options.cpp \
				progress.cpp \
				searcher.cpp \
				utils.cpp
CFLAGS=-c -Wall				
OBJECTS=$(patsubst %.cpp, build/%.o, $(SOURCES))
#OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=build/diskco

all: $(SOURCES) $(EXECUTABLE)

build:
	mkdir build
	
install: $(EXECUTABLE)
	install build/diskco $(INSTALL_DIR)
	
$(EXECUTABLE): build $(OBJECTS)
	$(CC) $(OBJECTS) -o $@
	
build/%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf build