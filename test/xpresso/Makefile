ROOT ?= $(realpath $(CURDIR)/../..)
PROJECTS ?= $(wildcard ../../examples/nxp/*-xpresso-*)
TARGET ?= Debug
DOCKER = docker run --rm -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= scaprile/xpresso
HEADLESS_BUILD = /usr/local/mcuxpressoide/ide/mcuxpressoide --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild

all: $(PROJECTS)
	$(DOCKER) $(IMAGE) $(HEADLESS_BUILD) -data workspace -removeAll workspace

$(PROJECTS): FORCE
	(make -C $@ && make -C $@ clean) || ( \
	PROJNAME=`xq -r .projectDescription.name $@/.project` && \
	($(DOCKER) $(IMAGE) $(HEADLESS_BUILD) -data workspace -import wizard -cleanBuild $$PROJNAME/$(TARGET) || true) && \
	test -f $(@F)/$(TARGET)/$$PROJNAME.axf )
FORCE:
