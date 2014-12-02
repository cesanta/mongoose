# Copyright (c) 2014 Cesanta Software
# All rights reserved

SUBDIRS = $(sort $(filter-out csharp/, $(dir $(wildcard */))))
X = $(SUBDIRS)

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

clean:
	for d in $(SUBDIRS) ; do $(MAKE) -C $$d clean ; done