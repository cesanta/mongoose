PROG ?= client                    # Program we are building
DELETE = rm -rf                   # Command to remove files
OUT ?= -o $(PROG)                 # Compiler argument for output file
CFLAGS = -W -Wall -Wextra -g -I.  # Build options
SOURCES = main.c mongoose.c  mongoose_fs.c

CFLAGS_EXTRA ?= -DMG_TLS=MG_TLS_BUILTIN

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_ENABLE_PACKED_FS=1

ifeq ($(OS),Windows_NT)   # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
  PROG ?= client.exe            # Use .exe suffix for the binary
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
  OUT ?= -o $(PROG)             # Build output
  MAKE += WINDOWS=1 CC=$(CC)
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES) Makefile
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

csr: FORCE
	openssl ecparam -noout -name prime256v1 -genkey -out key.pem
	openssl req -new -key key.pem -subj /CN=Mongoose -out crt.csr

mongoose_fs.c: ca.pem crt.pem key.pem
	node ../../../test/pack.js ca.pem crt.pem key.pem > $@

clean:
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM mbedtls

# For automated test purposes only
example: FORCE
	touch mongoose_fs.c

FORCE:
