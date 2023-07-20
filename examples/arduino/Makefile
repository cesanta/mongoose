DOCKER ?= docker run --platform linux/amd64 --rm $(ENV) -v $(CWD):$(CWD) -w $(CWD)
CWD ?= $(realpath $(CURDIR))

all:
example:
	true

arduino: ENV = -v $(CWD):/root
arduino:
	curl -sL http://downloads.arduino.cc/arduino-1.8.19-linux64.tar.xz | unxz | tar -xf -
	mv arduino-* $@

arduino-xiao-board:
	$(DOCKER) mdashnet/cc2 ./arduino/arduino --pref "boardsmanager.additional.urls=https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json" --save-prefs
	$(DOCKER) mdashnet/cc2 ./arduino/arduino --pref "compiler.warning_level=all" --save-prefs
	$(DOCKER) mdashnet/cc2 ./arduino/arduino --install-boards Seeeduino:samd

arduino-xiao: ENV = -v $(CWD):/root
arduino-xiao: arduino arduino-xiao-board
	rm -rf tmp; mkdir tmp
	cp w5500/w5500.ino tmp/tmp.ino
	cp ../../mongoose.[ch] w5500/mongoose_custom.h tmp/
	$(DOCKER) mdashnet/cc2 ./arduino/arduino --verbose --verify --board Seeeduino:samd:seeed_XIAO_m0 tmp/tmp.ino

arduino-nano: ENV = -v $(CWD)/arduino:/root
arduino-nano: arduino
	rm -rf tmp; mkdir tmp
	cp w5500/w5500.ino tmp/tmp.ino
	cp ../../mongoose.[ch] w5500/mongoose_custom.h tmp/
	$(DOCKER) mdashnet/cc2 ./arduino/arduino --verbose --verify --board arduino:avr:nano tmp/tmp.ino

clean: 
	rm -rf arduino tmp

