all: install
python_version_full := $(wordlist 2,3,$(subst ., ,$(shell python --version 2>&1)))
python_version_major := $(word 1,${python_version_full})

ifeq ($(python_version_major),2)
	python_executable = python3
else
	python_executable = python
endif

test:
	@echo ${python_executable}
clean:
	cd .. && $(python_executable) setup.py clean

install:
	cd .. && $(python_executable) setup.py install
