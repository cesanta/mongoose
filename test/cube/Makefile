ROOT ?= $(realpath $(CURDIR)/../..)
PROJECTS ?= $(wildcard ../../tutorials/stm32/nucleo-*-cube-*)
TARGET ?= Debug
DOCKER = docker run --rm -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= scaprile/cubeozone
# set PATHTO if image author did not set path

all: $(PROJECTS)
	$(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -removeAll workspace
	$(DOCKER) $(IMAGE) /bin/bash -c "rm -rf workspace/.metadata"

$(PROJECTS): FORCE
	(make -C $@ && sudo make -C $@ clean) || ( \
	COREDIRS=`find $@ -maxdepth 1 -name 'CM*' -print` && ( \
	(test "$$COREDIRS" = "" && \
	  PROJNAME=`xq -r .projectDescription.name $@/.project` && \
	  $(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -import $@ -cleanBuild $$PROJNAME/$(TARGET) && \
	  $(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -remove $@ && \
	  $(DOCKER) $(IMAGE) /bin/bash -c "rm -rf $@/.settings $@/$(TARGET)") || ( \
	(test "$$COREDIRS" != "" && \
	  for core in $$COREDIRS; do \
	    DIR=`basename $$core` \
	    PROJNAME=`xq -r .projectDescription.name $@/$$DIR/.project` && \
	    $(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -import $@/$$DIR -cleanBuild $$PROJNAME/$(TARGET) && \
	    $(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -remove $@/$$DIR && \
	    $(DOCKER) $(IMAGE) /bin/bash -c "rm -rf $@/$$DIR/.settings $@/$$DIR/Drivers $@/$$DIR/Middlewares $@/$$DIR/$(TARGET)" || exit 1; \
	  done ))))

FORCE:
