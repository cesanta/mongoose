# Copyright (c) 2014 Cesanta Software
# All rights reserved

# `wildcard ./*/` works in both linux and linux/wine, while `wildcard */` enumerates nothing under wine
SUBDIRS = $(sort $(dir $(wildcard ./*/)))
SUBDIRS:=$(filter-out ./ , $(SUBDIRS))


.PHONY: $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

clean:
	for d in $(SUBDIRS) ; do $(MAKE) -C $$d clean ; done
