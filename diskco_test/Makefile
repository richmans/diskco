all: a.out run
	
clean:
	rm -f catch.hpp.gch
	rm -f a.out
	
a.out:
	g++ *.?pp ../diskco/options.cpp ../diskco/byte_swapper.cpp ../diskco/utils.cpp ../diskco/buffer.cpp ../diskco/buffer_processor.cpp -I ../diskco
	
run:
	./a.out