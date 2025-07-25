PROG ?= ./example       # Program we are building
DELETE = rm -rf         # Command to remove files
OUT ?= -o $(PROG)       # Compiler argument for output file
SOURCES = main.c mongoose.c net.c packed_fs.c   # Source code files
CFLAGS = -W -Wall -Wextra -g -I.                # Build options

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

# Bundle JS libraries (preact, preact-router, ...) into a single file
web_root/bundle.js:
	curl -s https://npm.reversehttp.com/preact,preact/hooks,htm/preact,preact-router -o $@

# Generate packed filesystem for serving Web UI
packed_fs.c: $(wildcard web_root/*) $(wildcard certs/*) Makefile web_root/bundle.js
	node pack.js $(addsuffix ::gzip, $(wildcard web_root/*)) certs/* > $@

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
