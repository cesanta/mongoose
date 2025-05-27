# Wi-Fi chip firmware
mbed:
	git clone --depth 1 --no-checkout -b extrapatches-6.17.0 https://github.com/arduino/mbed-os $@ && cd $@ && git sparse-checkout set targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD/resources && git checkout
	touch wiced_resource.h

clean:
	rm -rf mbed
