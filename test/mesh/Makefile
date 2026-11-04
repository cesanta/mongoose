# Mesh simulator. `make` builds and runs an example
ROOT = ../..
SRCS = simulator.c l2_mesh.c l2_802154.c star.c $(ROOT)/mongoose.c
HDRS = simulator.h l2_mesh.h l2_802154.h star.h $(ROOT)/mongoose.h
WARN = -pedantic -W -Wall -Werror -Wshadow -Wdouble-promotion -fno-common \
       -Wconversion -Wundef -Wmissing-prototypes -Wstrict-prototypes
DEFS = -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_TCPIP_DRIVER_INIT=0 -DMG_ENABLE_IPV6=0 \
       -DMG_ENABLE_CUSTOM_MILLIS=1
CFLAGS ?= -O2 -g0 $(WARN)
X ?= 100
Y ?= 100
ROWS ?= 10
COLS ?= 10

# Nodes at every corner of a ROWS x COLS grid over X,Y, edges included
GRID = $(shell for r in $$(seq 0 $(ROWS)); do \
         for c in $$(seq 0 $(COLS)); do \
         echo -node $$((c * $(X) / $(COLS))),$$((r * $(Y) / $(ROWS))); \
         done; done)

all: mesh_sim
	./mesh_sim -area $(X),$(Y) \
	  -bridge $$(($(X) / 2)),$$(($(Y) / 2)) \
		$(GRID)

mesh_sim: $(SRCS) $(HDRS) Makefile
	$(CC) $(SRCS) $(CFLAGS) $(DEFS) -I. -I$(ROOT) -o $@

clean:
	rm -rf mesh_sim mesh_sim.dSYM

.PHONY: all clean
