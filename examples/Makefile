# Copyright (c) 2014 Cesanta Software
# All rights reserved

SUBDIRS = $(sort $(filter-out csharp/, $(dir $(wildcard */))))
X = $(SUBDIRS)
ifdef WINDIR
    CLIBS := -lws2_32
endif

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) CLIBS="$(CLIBS)" -C $@

clean:
	for d in $(SUBDIRS) ; do $(MAKE) -C $$d clean ; done
