SPROG ?= server                   # Program we are building
CPROG ?= client                   # Program we are building
DELETE = rm -rf                   # Command to remove files
SOUT ?= -o $(SPROG)               # Compiler argument for output file
COUT ?= -o $(CPROG)               # Compiler argument for output file
SSOURCES = server.c mongoose.c    # Source code files
CSOURCES = client.c mongoose.c    # Source code files
CFLAGS = -W -Wall -Wextra -g -I.  # Build options

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
#CFLAGS_MONGOOSE += -DMG_ENABLE_LINES

ifeq ($(OS),Windows_NT)   # Windows settings. Assume MinGW compiler. To use VC: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
  SPROG ?= server.exe             # Use .exe suffix for the binary
  CPROG ?= client.exe             # Use .exe suffix for the binary
  CC = gcc                        # Use MinGW gcc compiler
  CFLAGS += -lws2_32              # Link against Winsock library
  DELETE = cmd /C del /Q /F /S    # Command prompt command to delete files
  SOUT ?= -o $(SPROG)             # Build output
  COUT ?= -o $(CPROG)             # Build output
endif

all: example                      # Default target. Build all and run server
	$(RUN) ./$(SPROG) $(SARGS)

example: $(SPROG) $(CPROG)            

$(SPROG): $(SSOURCES)       # Build program from sources
	$(CC) $(SSOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(SOUT)

$(CPROG): $(CSOURCES)       # Build program from sources
	$(CC) $(CSOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(COUT)

clean:                      # Cleanup. Delete built program and all build artifacts
	$(DELETE) $(SPROG) $(CPROG) *.o *.obj *.exe *.dSYM
