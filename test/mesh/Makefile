# Mesh simulator. `make` builds and runs an example
ROOT = ../..
PROG ?= simulator
SRCS = simulator.c mesh_a.c $(ROOT)/mongoose.c
HDRS = simulator.h $(ROOT)/mongoose.h
WARN = -pedantic -W -Wall -Werror -Wshadow -Wdouble-promotion -fno-common \
       -Wconversion -Wundef -Wmissing-prototypes -Wstrict-prototypes
DEFS = -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_TCPIP_DRIVER_INIT=0 -DMG_ENABLE_IPV6=0 \
       -DMG_ENABLE_CUSTOM_MILLIS=1 -DMG_ENABLE_TCPIP_TCPRTX=1 \
       -DMG_IO_SIZE=512 \
       -DMG_TCPIP_ARP_MS=1000 -DMG_TCPIP_ACK_MS=500  # Slow radio: wait longer
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

all: $(PROG)
	$(RUN) ./$(PROG) -area $(X),$(Y) \
	  -bridge $$(($(X) / 2)),$$(($(Y) / 2)) \
		$(GRID) $(ARGS)

$(PROG): $(SRCS) $(HDRS) Makefile
	$(CC) $(SRCS) $(CFLAGS) $(DEFS) -I. -I$(ROOT) -o $@

clean:
	rm -rf $(PROG) $(PROG).dSYM *.exe

.PHONY: all clean
