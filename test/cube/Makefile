ROOT ?= $(realpath $(CURDIR)/../..)
PROJECTS ?= $(wildcard ../../examples/stm32/nucleo-*-cube-*)
TARGET ?= Debug
DOCKER = docker run --rm -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= scaprile/cubeozone
# set PATHTO if image author did not set path

all: $(PROJECTS)
	$(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -removeAll workspace

$(PROJECTS): FORCE
	(make -C $@ && make -C $@ clean) || ( \
	PROJNAME=`xq -r .projectDescription.name $@/.project` && \
	$(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -import $@ -cleanBuild $$PROJNAME/$(TARGET) )

FORCE:
