SOURCES = mongoose.c main.c net.c packed_fs.c
CFLAGS = -W -Wall -Wextra -g

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS += -DMG_ENABLE_PACKED_FS=1

ifeq ($(OS),Windows_NT)
  # Windows settings. Assume MinGW compiler
  PROG ?= example.exe           # Use .exe suffix for the binary
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
else
  # Mac, Linux
  PROG ?= example
  DELETE = rm -rf
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

# Before embedding files, gzip them to save space
packed_fs.c: $(shell find web_root -type f) Makefile
	rm -rf tmp/web_root && mkdir tmp && cp -r web_root tmp/
	find tmp -type f | xargs -n1 gzip
	$(CC) ../../test/pack.c -o pack
	cd tmp && ../pack `find web_root -type f` > ../$@

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) -o $@

clean:
	$(DELETE) $(PROG) *.dSYM pack tmp
