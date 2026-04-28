# Environment setup: https://mongoose.ws/docs/getting-started/build-environment/

PROG ?= example
TOP ?= ../../..
SOURCES = main.c dashboard.c file_data.c $(TOP)/mongoose.c
CFLAGS ?= -W -Wall -Wextra -Werror -Wundef -Wshadow -O0
PFLAGS = -I$(TOP) -DMG_DATA_SIZE=64 -DMG_TLS=MG_TLS_BUILTIN -DMG_ENABLE_LINES=1
AFLAGS ?= -fsanitize=signed-integer-overflow,address,undefined,alignment

all: $(PROG)
	./$(PROG)

# Rebuild the program
$(PROG): $(SOURCES) $(TOP)/mongoose.h
	$(CC) $(SOURCES) $(CFLAGS) $(PFLAGS) $(AFLAGS) -o $@

# Create an embedded filesystem containing compressed dashboard.html
file_data.c: dashboard.html
	node $(TOP)/resources/inline.js dashboard.html -o out.html
	node $(TOP)/resources/pack.js out.html:dashboard.html:gzip > file_data.c
	@echo "On-flash size: `grep -oE '[0-9]+' file_data.c | wc -l`"

purge:
	curl -s https://purge.jsdelivr.net/gh/cesanta/mongoose@master/resources/dashboard.js

# For development: rebuild and restart if any of the sources change
watch:
	node $(TOP)/resources/watch.js -w main.c -w dashboard.c -w $(TOP)/mongoose.h -w dashboard.html -- make

clean:
	rm -rf $(PROG) *.exe *.obj *.dSYM mongoose out.* file_data.c
