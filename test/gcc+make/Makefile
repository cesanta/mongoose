PROJECTS ?= $(wildcard ../../tutorials/*/*-make-*)

all: $(PROJECTS)
	echo

$(PROJECTS): FORCE
	$(MAKE) -C $@ build clean

FORCE:
