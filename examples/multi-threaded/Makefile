SOURCES = main.c mongoose.c       # Source code files
CFLAGS = -W -Wall -Wextra -g -I.  # Build options

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS_MONGOOSE +=

ifeq ($(OS),Windows_NT)
  # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD
  PROG ?= example.exe                 # Use .exe suffix for the binary
  CC = gcc                            # Use MinGW gcc compiler
  CFLAGS += -lws2_32                  # Link against Winsock library
  CFLAGS += -Wno-cast-function-type   # Thread functions return void instead of void *
  DELETE = cmd /C del /f /q /s        # Command prompt command to delete files
else
  # Mac, Linux
  PROG ?= example
  CFLAGS += -lpthread                 # Link against POSIX threads library
  DELETE = rm -rf
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) -o $@

clean:
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM
