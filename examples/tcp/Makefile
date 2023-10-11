PROG ?= example                   # Program we are building
PACK ?= ./pack                    # Packing executable
DELETE = rm -rf                   # Command to remove files
OUT ?= -o $(PROG)                 # Compiler argument for output file
SOURCES = main.c mongoose.c packed_fs.c       # Source code files
CFLAGS = -W -Wall -Wextra -g -I.  # Build options

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_ENABLE_PACKED_FS=1

ifeq ($(OS),Windows_NT)   # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
  PROG ?= example.exe           # Use .exe suffix for the binary
  PACK = pack.exe               # Packing executable
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
  OUT ?= -o $(PROG)             # Build output
  MAKE += WINDOWS=1 CC=$(CC)
endif

all: $(PROG)              # Default target. Build and run program
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES)       # Build program from sources
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

clean:                    # Cleanup. Delete built program and all build artifacts
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM mbedtls $(PACK)

# Generate packed filesystem for serving credentials
packed_fs.c: $(wildcard certs/*) Makefile
	$(CC) ../../test/pack.c -o $(PACK)
	$(PACK) $(wildcard certs/*) > $@

# see https://mongoose.ws/tutorials/tls/#how-to-build for TLS build options

mbedtls:                  # Pull and build mbedTLS library
	git clone --depth 1 -b v2.28.2 https://github.com/mbed-tls/mbedtls $@
	$(MAKE) -C mbedtls/library
