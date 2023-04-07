ROOT ?= $(realpath $(CURDIR)/../..)
PROJECTS = $(wildcard ../../examples/stm32/nucleo-*-cube-*)
DOCKER = docker run --rm -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= springrockconsulting/stm32cubeide:v1
# image author did not set path, so a hardcoded reference to a variable name dir is needed

all: $(PROJECTS)
	$(DOCKER) $(IMAGE) /opt/st/stm32cubeide_1.12.0/headless-build.sh -data workspace -removeAll workspace

$(PROJECTS): FORCE
	$(DOCKER) $(IMAGE) /opt/st/stm32cubeide_1.12.0/headless-build.sh -data workspace -import $@ -cleanBuild $(@F)

FORCE:
