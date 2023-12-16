PROG ?= ./example       # Program we are building
PACK ?= ./pack          # Packing executable
DELETE = rm -rf         # Command to remove files
GZIP ?= gzip            # For compressing files in web_root/
OUT ?= -o $(PROG)       # Compiler argument for output file
SOURCES = main.c mongoose.c net.c packed_fs.c   # Source code files
CFLAGS = -W -Wall -Wextra -g -I.                # Build options
NPX ?= npx

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_ENABLE_PACKED_FS=1

ifeq ($(OS),Windows_NT)         # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
  PROG = example.exe            # Use .exe suffix for the binary
  PACK = pack.exe               # Packing executable
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
  GZIP = echo                   # No gzip on Windows
endif

# Default target. Build and run program
all: $(PROG)
	$(RUN) $(PROG) $(ARGS)

# Build program from sources
$(PROG): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

# Bundle JS libraries (preact, preact-router, ...) into a single file
web_root/bundle.js:
	curl -s https://npm.reversehttp.com/preact,preact/hooks,htm/preact,preact-router -o $@

# Create optimised CSS. Prerequisite: npm -g i tailwindcss tailwindcss-font-inter
web_root/main.css: web_root/index.html $(wildcard web_root/*.js)
	$(NPX) tailwindcss -o $@ --minify

# Generate packed filesystem for serving Web UI
packed_fs.c: $(wildcard web_root/*) $(wildcard certs/*) Makefile web_root/main.css web_root/bundle.js
	$(GZIP) web_root/*
	$(CC) ../../test/pack.c -o $(PACK)
	$(PACK) web_root/* certs/* > $@
	$(GZIP) -d web_root/*

mbedtls:
	git clone --depth 1 -b v2.28.2 https://github.com/mbed-tls/mbedtls $@

ifeq ($(TLS), mbedtls)
CFLAGS += -DMG_TLS=MG_TLS_MBED -Wno-conversion -Imbedtls/include
CFLAGS += -DMBEDTLS_CONFIG_FILE=\"mbedtls_config.h\" mbedtls/library/*.c
$(PROG): mbedtls
endif

# Cleanup. Delete built program and all build artifacts
clean:
	$(DELETE) $(PROG) $(PACK) *.o *.obj *.exe *.dSYM mbedtls
