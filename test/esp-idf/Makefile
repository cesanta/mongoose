ROOT ?= $(realpath $(CURDIR)/../..)
PROJECTS ?= $(wildcard ../../examples/esp32/*)
TARGET ?= Debug
DOCKER = docker run --rm -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= espressif/idf


all: $(PROJECTS)
	echo

$(PROJECTS): FORCE
	(make -C $@ build && make -C $@ clean) || ( \
	$(DOCKER) $(IMAGE) /bin/bash -c 'cd $@ && idf.py build' && \
	rm -rf build sdkconfig )

FORCE:	
