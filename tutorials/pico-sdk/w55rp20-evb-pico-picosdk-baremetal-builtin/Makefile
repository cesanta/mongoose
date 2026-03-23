BOARD = evb-pico-w55rp20
IDE = Pico-SDK
RTOS = baremetal
WIZARD_URL ?= http://mongoose.ws/wizard

all build example: firmware.uf2

firmware.uf2: wizard
	make -C wizard CFLAGS_EXTRA=$(CFLAGS_EXTRA) && mv wizard/build/firmware.uf2 ./

wizard:
	hash=$$(curl -H 'Cookie: mongoose_wizard_uid=F2PPlAgQCqSthV1g' -s -X POST -H "Content-Type: application/json" -d '{"build":{"board":"$(BOARD)","ide":"$(IDE)","rtos":"$(RTOS)"},"http":{"http": true}}' $(WIZARD_URL)/api/hash | jq -r '.hash') \
	&& curl -H 'Cookie: mongoose_wizard_uid=F2PPlAgQCqSthV1g' -s $(WIZARD_URL)/api/zip -o wizard.zip
	unzip wizard.zip
	cd wizard/mongoose ; rm mongoose.[ch] ; ln -s ../../../../../mongoose.c ; ln -s ../../../../../mongoose.h

flash: firmware.uf2
	picotool load $< -f

clean:
	rm -rf firmware.* wizard* pico-sdk

