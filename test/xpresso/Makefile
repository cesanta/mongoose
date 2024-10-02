ROOT ?= $(realpath $(CURDIR)/../..)
PROJECTS ?= $(wildcard ../../examples/nxp/*-xpresso-*)
TARGET ?= Debug
DOCKER = docker run --rm -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= scaprile/xpresso
HEADLESS_BUILD = /usr/local/mcuxpressoide/ide/mcuxpressoide --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild

all: $(PROJECTS)
	$(DOCKER) $(IMAGE) $(HEADLESS_BUILD) -data workspace -removeAll workspace

$(PROJECTS): FORCE
	$(DOCKER) $(IMAGE) $(HEADLESS_BUILD) -data workspace -import $@ -cleanBuild $(@F)/$(TARGET)

FORCE:

# Automated remote test. See https://vcon.io/automated-firmware-tests/
URL ?= https://dash.vcon.io/api/v3/devices
update: $(PROJECTS)
	curl --fail-with-body -su :$(VCON_API_KEY) $(URL)/$(DEVICE)/ota --data-binary @$</$(TARGET)/firmware.bin

test update: TARGET = Test
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(URL)/$(DEVICE)/tx?t=5 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init
#	grep 'MQTT connected' /tmp/output.txt   # Check for MQTT connection success
