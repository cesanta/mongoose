ROOT ?= $(realpath $(CURDIR)/../..)
TARGET ?= Debug
DOCKER = docker run --rm -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= scaprile/cgt

all: $(PROJECTS)
	echo

$(PROJECTS): FORCE
	$(DOCKER) $(IMAGE) make -C $@

FORCE:
