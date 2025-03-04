PROJECTS ?= arduino-xiao #arduino-nano #arduino-esp32 arduino-teensy41
ROOT ?= $(realpath $(CURDIR)/../..)
ARD ?= $(realpath $(CURDIR))

all: $(PROJECTS)
	echo 

arduino: ENV = -v $(CWD):/root
arduino:
	curl -sL http://downloads.arduino.cc/arduino-1.8.19-linux64.tar.xz | unxz | tar -xf -
	mv arduino-* $@
	./arduino/arduino --pref "compiler.warning_level=all" --save-prefs

arduino-xiao-board:
	./arduino/arduino --pref "boardsmanager.additional.urls=https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json" --save-prefs
	./arduino/arduino --install-boards Seeeduino:samd

#esp32-board:
#	./arduino/arduino --pref "boardsmanager.additional.urls=https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json" --save-prefs
#	./arduino/arduino --install-boards esp32:esp32
#
#teensy-board:
#	./arduino/arduino --pref "boardsmanager.additional.urls=https://www.pjrc.com/teensy/package_teensy_index.json" --save-prefs
#	./arduino/arduino --install-boards teensy:avr

arduino-xiao: arduino arduino-xiao-board
	cd $(ROOT)/tutorials/mqtt/mqtt-client/arduino/sim800-mqtt && $(ARD)/arduino/arduino --verbose --verify --board Seeeduino:samd:seeed_XIAO_m0 sim800-mqtt.ino
	cd $(ROOT)/tutorials/http/http-server/arduino/w5500-http &&  $(ARD)/arduino/arduino --verbose --verify --board Seeeduino:samd:seeed_XIAO_m0 w5500-http.ino
	cd $(ROOT)/tutorials/mqtt/mqtt-client/arduino/w5500-mqtt &&  $(ARD)/arduino/arduino --verbose --verify --board Seeeduino:samd:seeed_XIAO_m0 w5500-mqtt.ino

arduino-nano: arduino
	cd $(ROOT)/tutorials/http/http-server/arduino/w5500-http && $(ARD)/arduino/arduino --pref compiler.cpp.extra_flags=-DMG_ENABLE_LOG=0 --verbose --verify --board arduino:avr:nano w5500-http.ino
	cd $(ROOT)/tutorials/mqtt/mqtt-client/arduino/w5500-mqtt && $(ARD)/arduino/arduino --pref compiler.cpp.extra_flags=-DMG_ENABLE_LOG=0 --verbose --verify --board arduino:avr:nano w5500-mqtt.ino

#arduino-esp32: arduino esp32-board
#	cd $(ROOT)/tutorials/http/http-server/arduino/esp32-http && $(ARD)/arduino/arduino --verbose --verify --board esp32-http.ino
#	cd $(ROOT)/tutorials/mqtt/mqtt-client/arduino/esp32-mqtt && $(ARD)/arduino/arduino --verbose --verify --board esp32-mqtt.ino
#
#arduino-teensy41: arduino teensy-board
#	cd $(ROOT)/tutorials/http/http-server/arduino/teensy41-http && $(ARD)/arduino/arduino --verbose --verify --board teensy41-http.ino



clean:
	rm -rf arduino 

cleanall: clean		#otherwise some parts of xiao-board installation will fail as already installed
	rm -rf ~/.arduino* ~/.oracle_jre_usage
