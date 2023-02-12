SOURCES = main.c mongoose.c       # Source code files
CFLAGS = -W -Wall -Wextra -g -I.  # Build options

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_HTTP_DIRLIST_TIME=1 -DMG_ENABLE_SSI=1
CFLAGS_MONGOOSE += -DMG_ENABLE_LINES=1 -DMG_ENABLE_IPV6=1

ifeq ($(OS),Windows_NT)
  # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
  PROG ?= example.exe           # Use .exe suffix for the binary
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /q /f /s  # Command prompt command to delete files
  OUT ?= -o $(PROG)             # Build output
else
  # Mac, Linux
  PROG ?= example
  DELETE = rm -rf
  OUT ?= -o $(PROG)
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

clean:
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM
