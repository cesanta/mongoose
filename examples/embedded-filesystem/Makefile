SOURCES = mongoose.c main.c packed_fs.c  # Source code files
CFLAGS = -W -Wall -Wextra -g -I.         # Build options

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_ENABLE_PACKED_FS=1

FILES_TO_EMBED ?= $(wildcard web_root/*)

ifeq ($(OS),Windows_NT)
  # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
  PROG ?= example.exe           # Use .exe suffix for the binary
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
  OUT ?= -o $(PROG)             # Build output
else
  # Mac, Linux
  PROG ?= example
  DELETE = rm -rf
  OUT ?= -o $(PROG)
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES) $(FILES_TO_EMBED)
	$(CC) ../../test/pack.c -o pack
	./pack $(FILES_TO_EMBED) > packed_fs.c
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

clean:
	$(DELETE) $(PROG) *.dSYM pack

.PHONY: compress expand gzipped plain all

# See tutorial
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

