# Environment setup: https://mongoose.ws/docs/getting-started/build-environment/

PROG ?= example
TOP ?= ../../..
SOURCES = dashboard.c file_data.c $(TOP)/mongoose.c
CFLAGS_EXTRA ?= -DMG_TLS=MG_TLS_BUILTIN
CFLAGS ?= -W -Wall -Wextra -Werror -Wundef -Wshadow -O0 $(CFLAGS_EXTRA)
PFLAGS = -I$(TOP) -DMG_DATA_SIZE=64 -DMG_ENABLE_LINES=1 -DMAIN
AFLAGS ?= -fsanitize=signed-integer-overflow,address,undefined,alignment

all: $(PROG)
	./$(PROG)

# Rebuild the program
$(PROG): $(SOURCES) $(TOP)/mongoose.h
	$(CC) $(SOURCES) $(CFLAGS) $(PFLAGS) $(AFLAGS) -o $@

# Create an embedded filesystem containing compressed dashboard.html
file_data.c: dashboard.html
	node $(TOP)/resources/html2c.js dashboard.html -o file_data.c
	@echo "On-flash size: `grep -oE '[0-9]+' file_data.c | wc -l`"

# For development: rebuild and restart if any of the sources change
watch:
	node $(TOP)/resources/watch.js -w dashboard.c -w $(TOP)/mongoose.h -w dashboard.html -- make

clean:
	rm -rf $(PROG) *.exe *.obj *.dSYM mongoose file_data.c
