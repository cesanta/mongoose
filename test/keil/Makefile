# Do NOT use parenthesis on Windows project dir names
PROJECTS ?= $(wildcard ../../examples/stm32/nucleo-*-keil-*)
TARGET ?= "Target 1"

all: $(PROJECTS)
	
# Need to test inside .bat to check and transfer errors
$(PROJECTS): FORCE
	unitest.bat $(subst /,\,$@) $(TARGET)

FORCE:

# Automated remote test. See https://vcon.io/automated-firmware-tests/
URL ?= https://dash.vcon.io/api/v3/devices
update: $(PROJECTS)
	curl -su :$(VCON_API_KEY) $(URL)/$(DEVICE)/ota?hex=1 --data-binary @$</Objects/firmware.hex

test update: TARGET = Test
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(URL)/$(DEVICE)/tx?t=5
# TODO(): no tee, no grep
#| tee /tmp/output.txt
#	grep 'READY, IP:' /tmp/output.txt       # Check for network init
