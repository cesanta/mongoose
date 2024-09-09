PROG ?= example
DELETE = rm -rf
OUT ?= -o $(PROG)
SOURCES = main.c mongoose.c
CFLAGS = -W -Wall -Wextra -g -I.
CFLAGS += -lpcap

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_ENABLE_LINES=1 -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_SOCKET=0
CFLAGS_MONGOOSE += -DMG_ENABLE_TCPIP_DRIVER_INIT=0
CFLAGS_EXTRA ?= -DMG_TLS=MG_TLS_BUILTIN

ifeq ($(OS),Windows_NT)   # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
  PROG ?= example.exe           # Use .exe suffix for the binary
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
  OUT ?= -o $(PROG)             # Build output
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES) Makefile
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

clean:
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM
