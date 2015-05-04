all: compile python
	
compile:
	cd diskco && $(MAKE)
	cd diskco_test && $(MAKE)

test:
	cd diskco_test && $(MAKE) test
	
python:
	cd diskco_python && $(MAKE)

install: compile
	cd diskco && $(MAKE) install
	
clean:
	rm -rf diskco/build
	rm -rf diskco_python/a.out
	rm -rf diskco_test/build
