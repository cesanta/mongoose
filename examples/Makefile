# Copyright (c) 2014 Cesanta Software
# All rights reserved

# `wildcard ./*/` works in both linux and linux/wine, while `wildcard */` enumerates nothing under wine
SUBDIRS = $(sort $(dir $(wildcard ./*/)))
SUBDIRS:=$(filter-out ./ ./CC3200/ ./ESP32_IDF/ ./ESP8266_RTOS/ ./mbed/ ./MSP432/ ./nRF51/ ./nRF52/ ./NXP_K64/ ./NXP_LPC4088/ ./PIC32/ ./STM32F4_CC3100/ ./TM4C129/ ./WinCE/, $(SUBDIRS))

ifeq ($(OS), Windows_NT)
  SUBDIRS:=$(filter-out ./netcat/ ./raspberry_pi_mjpeg_led/ ./captive_dns_server/, $(SUBDIRS))
endif

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

clean:
	for d in $(SUBDIRS) ; do $(MAKE) -C $$d clean ; done
