PROG ?= ./example                 # Program we are building
DELETE ?= rm -rf                  # Command to remove files
OUT ?= -o $(PROG)                 # Compiler argument for output file
SOURCES = main.c net.c mongoose.c # Source code files
CFLAGS ?= -W -Wall -Wextra -g -I. # Build options

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_ENABLE_LINES=1

ifeq ($(OS),Windows_NT)   # Windows settings for MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feexample.exe
  PROG = example.exe            # Use .exe suffix for the binary
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against the Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
endif

all: $(PROG)
	$(RUN) $(PROG) $(ARGS)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

clean:
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM
