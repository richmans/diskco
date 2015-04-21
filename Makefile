all: cmd python test
	
cmd:
	cd diskco && $(MAKE)

test:
	cd diskco_test && $(MAKE)
	
python:
	cd diskco_python && $(MAKE)
	
clean:
	rm -rf build
	