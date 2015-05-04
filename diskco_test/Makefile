CC=g++
SUT_SOURCES=../diskco/buffer.cpp \
				../diskco/buffer_pool.cpp \
				../diskco/buffer_processor.cpp \
				../diskco/byte_swapper.cpp \
  			../diskco/file_reader.cpp \
				../diskco/file_writer.cpp \
				../diskco/options.cpp \
				../diskco/progress.cpp \
				../diskco/searcher.cpp \
				../diskco/utils.cpp
SOURCES=mock_buffer_processor.cpp \
				testsuite.cpp \
				test_byte_swapper.cpp \
				test_file_reader.cpp \
				test_file_writer.cpp \
				test_options.cpp \
				test_utils.cpp \
				test.cpp
				
CFLAGS=-c -Wall		
SUT_DIR=../diskco
BUILD_DIR=build
# System Under Test code : The code that is being tested
SUT_OBJECTS=$(patsubst $(SUT_DIR)/%.cpp, $(BUILD_DIR)/sut/%.o, $(SUT_SOURCES))
# Test code : The code that is doing the testing
TEST_OBJECTS=$(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
EXECUTABLE=build/test_diskco

all: $(SOURCES) $(EXECUTABLE) run

build:
	mkdir $(BUILD_DIR)
	mkdir $(BUILD_DIR)/sut
	
$(EXECUTABLE): build catch $(SUT_OBJECTS) $(TEST_OBJECTS)
	$(CC) $(SUT_OBJECTS) $(TEST_OBJECTS) -o $@

catch: catch.hpp
	$(CC) catch.hpp -o $(BUILD_DIR)/catch.hpp.gch
	
build/%.o: %.cpp
	$(CC) $(CFLAGS) -I $(SUT_DIR) -I . -o $@ $<

build/sut/%.o: $(SUT_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

run: $(EXECUTABLE)
	$(EXECUTABLE)
	
clean: 
	rm -rf $(BUILD_DIR)