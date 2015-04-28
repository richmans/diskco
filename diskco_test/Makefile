all: a.out run
	
clean:
	rm -f catch.hpp.gch
	rm -f a.out
	
a.out:
	g++ -std=c++11 *.?pp ../diskco/buffer_pool.cpp ../diskco/options.cpp ../diskco/byte_swapper.cpp ../diskco/utils.cpp ../diskco/buffer.cpp ../diskco/buffer_processor.cpp -I ../diskco
	
run: a.out
	./a.out
