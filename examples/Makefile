# Copyright (c) 2014 Cesanta Software
# All rights reserved

SUBDIRS = $(sort $(dir $(wildcard */)))

ifeq ($(OS), Windows_NT)
  SUBDIRS:=$(filter-out load_balancer/ netcat/ raspberry_pi_mjpeg_led/, $(SUBDIRS))
endif

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

clean:
	for d in $(SUBDIRS) ; do $(MAKE) -C $$d clean ; done
