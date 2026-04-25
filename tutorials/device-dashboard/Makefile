# Environment setup: https://mongoose.ws/docs/getting-started/build-environment/

PROG ?= example
MONGOOSE_DIR ?= ../..
SOURCES = main.c mongoose_dashboard.c dashboard.html.c $(MONGOOSE_DIR)/mongoose.c
CFLAGS ?= -W -Wall -Wextra -Werror -Wundef -Wshadow -g3 -O0
PFLAGS = -I$(MONGOOSE_DIR) -DMG_DATA_SIZE=64 -DMG_TLS=MG_TLS_BUILTIN -DMG_ENABLE_LINES=1
AFLAGS ?= -fsanitize=signed-integer-overflow,address,undefined,alignment

all: $(PROG)
	./$(PROG)

# Rebuild the program
$(PROG): $(SOURCES) $(MONGOOSE_DIR)/mongoose.h
	$(CC) $(SOURCES) $(CFLAGS) $(PFLAGS) $(AFLAGS) -o $@

# Generate a single, self-contained dashboard.html from the UI in web_root/
dashboard.html: $(wildcard web_root/*)
	monolith web_root/index.html > $@

# Create an embedded filesystem containing compressed dashboard.html
dashboard.html.c: dashboard.html
	node $(MONGOOSE_DIR)/resources/pack.js dashboard.html::gzip > $@
	@echo "On-flash size: `grep -oE '[0-9]+' $@ | wc -l`"

# For development: rebuild and restart if any of the sources change
watch:
	watchexec -r -w main.c -w mongoose_dashboard.c -w $(MONGOOSE_DIR)/mongoose.h -w web_root -e c,h,html,css,js -- make

clean:
	rm -rf $(PROG) *.exe *.obj *.dSYM mongoose dashboard.*
