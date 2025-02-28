BOARD = rt1060
IDE = GCC+make
RTOS = baremetal
WIZARD_URL ?= http://mongoose.ws/wizard

all build example: firmware.bin

firmware.bin: wizard
	make -C wizard CFLAGS_EXTRA=$(CFLAGS_EXTRA) && mv wizard/firmware.bin ./

wizard:
	hash=$$(curl -s -X POST -H "Content-Type: application/json" -d '{"build":{"board":"$(BOARD)","ide":"$(IDE)","rtos":"$(RTOS)"}}' $(WIZARD_URL)/api/hash | jq -r '.hash') \
	&& curl -s $(WIZARD_URL)/api/zip/$(BOARD)/$(IDE)/$(RTOS)/$$hash -o wizard.zip
	unzip wizard.zip
	cd wizard ; rm mongoose.[ch] ; ln -s ../../../../mongoose.c ; ln -s ../../../../mongoose.h


# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/13
update: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<

test update: CFLAGS_EXTRA ="-DUART_DEBUG=LPUART3"
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=15 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init

clean:
	rm -rf firmware.* wizard*
