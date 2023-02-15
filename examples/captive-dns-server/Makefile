SOURCES = mongoose.c main.c
CFLAGS = -W -Wall -Wextra -g

# Mongoose build options. See https://mongoose.ws/documentation/#build-options
CFLAGS += -DMG_IO_SIZE=8192 -DMG_ENABLE_LINES

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
	$(DELETE) $(PROG) *.dSYM pack tmp
