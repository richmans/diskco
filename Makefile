all: cmd python test
	
cmd:
	cd diskco && $(MAKE)

test:
	cd diskco_test && $(MAKE)
	
python:
	cd diskco_python && $(MAKE)
	
clean:
	rm -rf diskco/build
	rm -rf diskco_python/a.out
	rm -rf diskco_test/build
