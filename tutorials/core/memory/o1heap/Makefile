SOURCES = main.c mongoose.c       # Source code files
CFLAGS = -W -Wall -Wextra -g -I.  # Build options

# O(1) Heap
SOURCES += o1heap/o1heap/o1heap.c
CFLAGS += -Io1heap/o1heap

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE += -DMG_ENABLE_CUSTOM_CALLOC

ifeq ($(OS),Windows_NT)
  # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD
  PROG ?= example.exe                 # Use .exe suffix for the binary
  CC = gcc                            # Use MinGW gcc compiler
  CFLAGS += -lws2_32                  # Link against Winsock library
  DELETE = cmd /C del /f /q /s        # Command prompt command to delete files
else
  # Mac, Linux
  PROG ?= example
  DELETE = rm -rf
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) -o $@

o1heap/o1heap/o1heap.c:  # O(1) Heap sources
	git clone --depth 1 -b 2.1.0 https://github.com/pavel-kirienko/o1heap.git

clean:
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM
