SOURCES = main.c ../../mongoose.c  # Source code files
CFLAGS = -W -Wall -Wextra -g       # Build options

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS += -I ../..
CFLAGS += -DMG_HTTP_DIRLIST_TIME=1 -DMG_ENABLE_SSI=1
CFLAGS += -DMG_ENABLE_LINES=1 -DMG_ENABLE_IPV6=1

ifeq ($(OS),Windows_NT)
  # Windows settings. Assume MinGW compiler
  PROG ?= example.exe           # Use .exe suffix for the binary
  CC = gcc                      # Use MinGW gcc compiler
  CFLAGS += -lws2_32            # Link against Winsock library
  DELETE = cmd /C del /Q /F /S  # Command prompt command to delete files
else
  # Mac, Linux
  PROG ?= example
  DELETE = rm -rf
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) -o $@

clean:
	$(DELETE) $(PROG) *.o *.dSYM
