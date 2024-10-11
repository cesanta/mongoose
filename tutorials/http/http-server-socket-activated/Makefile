PROG ?= example                   # Program we are building
DELETE = rm -rf                   # Command to remove files
OUT ?= -o $(PROG)                 # Compiler argument for output file
SOURCES = main.c mongoose.c       # Source code files
CFLAGS = -W -Wall -Wextra -g -I.  # Build options
CFLAGS += -lsystemd

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_HTTP_DIRLIST_TIME_FMT="%Y/%m/%d %H:%M:%S"
CFLAGS_MONGOOSE += -DMG_ENABLE_LINES=1 -DMG_ENABLE_IPV6=1 -DMG_ENABLE_SSI=1
CFLAGS_EXTRA ?= -DMG_TLS=MG_TLS_BUILTIN

all: $(PROG)              # Default target. Build and run program
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES)       # Build program from sources
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

vc98:
	cl $(SOURCES) -DMG_ENABLE_SSI=1 -DMG_TLS=MG_TLS_BUILTIN

clean:                    # Cleanup. Delete built program and all build artifacts
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM
