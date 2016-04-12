# Copyright (c) 2014 Cesanta Software
# All rights reserved

# `wildcard ./*/` works in both linux and linux/wine, while `wildcard */` enumerates nothing under wine
SUBDIRS = $(sort $(dir $(wildcard ./*/)))
SUBDIRS:=$(filter-out ./ ./CC3200/ ./ESP8266_RTOS/ ./MSP432/, $(SUBDIRS))

ifeq ($(OS), Windows_NT)
  SUBDIRS:=$(filter-out ./load_balancer/ ./netcat/ ./raspberry_pi_mjpeg_led/ ./captive_dns_server/, $(SUBDIRS))
endif

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

clean:
	for d in $(SUBDIRS) ; do $(MAKE) -C $$d clean ; done
