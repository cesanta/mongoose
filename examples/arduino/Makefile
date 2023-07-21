
all:
example:
	true

arduino: ENV = -v $(CWD):/root
arduino:
	curl -sL http://downloads.arduino.cc/arduino-1.8.19-linux64.tar.xz | unxz | tar -xf -
	mv arduino-* $@

arduino-xiao-board:
	./arduino/arduino --pref "boardsmanager.additional.urls=https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json" --save-prefs
	./arduino/arduino --pref "compiler.warning_level=all" --save-prefs
	./arduino/arduino --install-boards Seeeduino:samd

arduino-xiao: arduino arduino-xiao-board
	cd w5500-http && ../arduino/arduino --verbose --verify --board Seeeduino:samd:seeed_XIAO_m0 w5500-http.ino
	cd w5500-mqtt && ../arduino/arduino --verbose --verify --board Seeeduino:samd:seeed_XIAO_m0 w5500-mqtt.ino

arduino-nano: arduino
	cd w5500-http && ../arduino/arduino --pref compiler.cpp.extra_flags=-DMG_ENABLE_LOG=0 --verbose --verify --board arduino:avr:nano w5500-http.ino
	cd w5500-mqtt && ../arduino/arduino --pref compiler.cpp.extra_flags=-DMG_ENABLE_LOG=0 --verbose --verify --board arduino:avr:nano w5500-mqtt.ino

clean:
	rm -rf arduino 

cleanall: clean		#otherwise some parts of xiao-board installation will fail as already installed
	rm -rf ~/.arduino15 ~/.oracle_jre_usage
