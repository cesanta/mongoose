BOARD = pico-w
IDE = Pico-SDK
RTOS = baremetal
WIZARD_URL ?= http://mongoose.ws/wizard

all build example: firmware.uf2

firmware.uf2: wizard
	make -C wizard CFLAGS_EXTRA=$(CFLAGS_EXTRA) && mv wizard/build/firmware.uf2 ./

wizard:
	hash=$$(curl -s -X POST -H "Content-Type: application/json" -d '{"build":{"board":"$(BOARD)","ide":"$(IDE)","rtos":"$(RTOS)"}}' $(WIZARD_URL)/api/hash | jq -r '.hash') \
	&& curl -s $(WIZARD_URL)/api/zip/$(BOARD)/$(IDE)/$(RTOS)/$$hash -o wizard.zip
	unzip wizard.zip
	cd wizard ; rm mongoose.[ch] ; ln -s ../../../../mongoose.c ; ln -s ../../../../mongoose.h

flash: firmware.uf2
	picotool load $< -f

# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/??
# stdio has to be changed from USB to UART in CMakeLists.txt
update: wizard
	sed -i -e 's/pico_enable_stdio_usb(firmware 1)/pico_enable_stdio_usb(firmware 0)/' wizard/CMakeLists.txt
	(test `grep -c "pico_enable_stdio_uart(firmware 1)" wizard/CMakeLists.txt` -eq 0 && echo "pico_enable_stdio_uart(firmware 1)" >> wizard/CMakeLists.txt) || true
	make -C wizard CFLAGS_EXTRA=$(CFLAGS_EXTRA) && mv wizard/build/firmware.uf2 ./
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota?uf2=1 --data-binary @firmware.uf2

test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=15 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init

clean:
	rm -rf firmware.* wizard* pico-sdk

