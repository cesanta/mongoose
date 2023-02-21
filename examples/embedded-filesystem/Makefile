PROG ?= example                   # Program we are building
DELETE = rm -rf                   # Command to remove files
OUT ?= -o $(PROG)                 # Compiler argument for output file
SOURCES = main.c mongoose.c packed_fs.c      # Source code files
CFLAGS = -W -Wall -Wextra -g -I.  # Build options
PACK = ./pack             # Utility to pack files into a packed filesystem

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_ENABLE_PACKED_FS=1

FILES_TO_EMBED ?= $(wildcard web_root/*)

ifeq ($(OS),Windows_NT)   # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
  PROG ?= example.exe           # Use .exe suffix for the binary
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
  OUT ?= -o $(PROG)             # Build output
  PACK = pack.exe
endif

all: $(PROG)              # Default target. Build and run program
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES) $(FILES_TO_EMBED)       # Build program from sources
	$(CC) ../../test/pack.c -o $(PACK)
	$(PACK) $(FILES_TO_EMBED) > packed_fs.c
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

clean:                    # Cleanup. Delete built program and all build artifacts
	$(DELETE) $(PROG) *.dSYM $(PACK)

.PHONY: compress expand gzipped plain all

# See tutorial at https://mongoose.ws/tutorials/embedded-filesystem/#build-and-try
gzipped: compress
	$(MAKE) all

plain: expand
	$(MAKE) all

compress:
	for file in $(FILES_TO_EMBED) ; do \
		gzip -q $$file ; \
	done

expand:
	for file in $(FILES_TO_EMBED) ; do \
		gunzip -q $$file ; \
	done

