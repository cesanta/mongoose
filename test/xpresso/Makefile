# NXP command line tool is useless for this task, using Eclipse headless build

ROOT ?= $(realpath $(CURDIR)/../..)
PROJECTS ?= $(wildcard ../../tutorials/nxp/*-xpresso-*)
TARGET ?= Debug
DOCKER = docker run --rm --ulimit nofile=1024:524288 -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= scaprile/xpresso
HEADLESS_BUILD = /usr/local/mcuxpressoide/ide/mcuxpressoide --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild

all: $(PROJECTS)
	$(DOCKER) $(IMAGE) $(HEADLESS_BUILD) -data workspace -removeAll workspace
	$(DOCKER) $(IMAGE) /bin/bash -c "rm -rf workspace/.metadata workspace/.mcuxpressoide* workspace/.gitignore mcuxpresso .cache .p2 .eclipse"

$(PROJECTS): FORCE
	(make -C $@ && make -C $@ clean) || ( \
	PROJNAME=`xq -r .projectDescription.name $@/.project` && \
	($(DOCKER) $(IMAGE) $(HEADLESS_BUILD) -data workspace -import $@ -cleanBuild $$PROJNAME/$(TARGET) || true) && \
	test -f $(@F)/$(TARGET)/$$PROJNAME.axf && \
	$(DOCKER) $(IMAGE) /bin/bash -c "find $@/$(TARGET) -mindepth 1 ! -name '*.ld' -exec rm -rf {} +")

FORCE:
