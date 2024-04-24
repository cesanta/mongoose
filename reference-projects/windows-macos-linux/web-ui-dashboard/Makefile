# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

PROG ?= ./example       # Program we are building
DELETE = rm -rf         # Command to remove files
NPX ?= npx              # For generating optimised tailwind CSS
OUT ?= -o $(PROG)       # Compiler argument for output file

SOURCES = main.c mongoose.c net.c packed_fs.c
CFLAGS += -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion -Wconversion -Wno-sign-conversion
CFLAGS +=  -fno-common -ffunction-sections -fdata-sections
CFLAGS += -g3 -Os -I.

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_ENABLE_PACKED_FS=1

ifeq ($(OS),Windows_NT)         # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
  PROG = example.exe            # Use .exe suffix for the binary
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
endif

# Default target. Build and run program
all: $(PROG)
	$(RUN) $(PROG) $(ARGS)

# Build program from sources
$(PROG): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

# Build using Microsoft Visual C compiler
vc: $(SOURCES) Makefile
	cl $(SOURCES) /DMG_ENABLE_PACKED_FS=1 /MD /W3 /nologo /Fedemo.exe

# Bundle JS libraries (preact, preact-router, ...) into a single file
web_root/bundle.js:
	curl -s https://npm.reversehttp.com/preact,preact/hooks,htm/preact,preact-router -o $@

# Create optimised CSS. Prerequisite: npm -g i tailwindcss tailwindcss-font-inter
web_root/main.css: web_root/index.html $(wildcard web_root/*.js)
	$(NPX) tailwindcss -o $@ --minify

# Generate packed filesystem for serving Web UI
packed_fs.c: $(wildcard web_root/*) $(wildcard certs/*) Makefile
	node pack.js $(addsuffix ::gzip, $(wildcard web_root/*)) certs/* > $@

# Cleanup. Delete built program and all build artifacts
clean:
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM mbedtls
