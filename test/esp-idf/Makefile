ROOT ?= $(realpath $(CURDIR)/../..)
PROJECTS ?= ../../tutorials/esp32/device-dashboard/ ../../tutorials/micropython/esp32/ ../../tutorials/esp32/uart-bridge/
TARGET ?= Debug
DOCKER = docker run --rm -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= espressif/idf


all: $(PROJECTS)
	echo

$(PROJECTS): FORCE
	(make -C $@ build && make -C $@ clean) || ( \
	$(DOCKER) $(IMAGE) /bin/bash -c 'cd $@ && idf.py build' && \
	$(DOCKER) $(IMAGE) /bin/bash -c 'rm -rf .cache && cd $@ && rm -rf build sdkconfig')

FORCE:	
