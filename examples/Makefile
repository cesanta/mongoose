# Copyright (c) 2014 Cesanta Software
# All rights reserved

SUBDIRS = $(sort $(filter-out csharp/, $(dir $(wildcard */))))
X = $(SUBDIRS)
ifdef WINDIR
    # appending the Winsock2 library at the end of the compiler
	# invocation
    CFLAGS_EXTRA += -lws2_32
endif

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) CFLAGS_EXTRA="$(CFLAGS_EXTRA)" -C $@

clean:
	for d in $(SUBDIRS) ; do $(MAKE) -C $$d clean ; done
