all example build obj/firmware.uf2: obj pico-sdk main.c net.c packed_fs.c
	cd obj && cmake .. && make

obj:
	mkdir $@
pico-sdk:
	git clone --depth 1 -b 1.5.0 https://github.com/raspberrypi/pico-sdk $@
	cd $@ && git submodule update --init

# Requires env variable VCON_API_KEY set
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/3
test: update
	curl --fail -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'Ethernet: up' /tmp/output.txt
	grep 'MQTT connected' /tmp/output.txt

update: obj/firmware.uf2
	curl --fail -su :$(VCON_API_KEY) $(DEVICE_URL)/ota?uf2=1 --data-binary @$<

clean:
	rm -rf pico-sdk obj
