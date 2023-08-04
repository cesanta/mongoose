ROOT ?= $(realpath $(CURDIR)/../..)
PROJECTS ?= $(wildcard ../../examples/stm32/nucleo-*-cube-*)
TARGET ?= Debug
DOCKER = docker run --rm -v $(ROOT):$(ROOT) -v $(CURDIR):/root -w $(CURDIR) 
IMAGE ?= springrockconsulting/stm32cubeide:v1
# image author did not set path, so a hardcoded reference to a variable name dir is needed
PATHTO ?= /opt/st/stm32cubeide_1.12.0/

all: $(PROJECTS)
	$(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -removeAll workspace

$(PROJECTS): FORCE
	$(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -import $@ -cleanBuild $(@F)/$(TARGET)

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
