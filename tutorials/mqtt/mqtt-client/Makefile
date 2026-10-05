REPO_ROOT ?= ../../..
PROG ?= example
DELETE = rm -rf
OUT ?= -o $(PROG)
CFLAGS = -W -Wall -Wextra -g -I. -I$(REPO_ROOT)
SOURCES = main.c mongoose_mqtt.c $(REPO_ROOT)/mongoose.c

# Mongoose build options. See https://mongoose.ws/docs/getting-started/build-options/
CFLAGS_MONGOOSE += -DMG_TLS=MG_TLS_BUILTIN
CFLAGS_MONGOOSE += -DMG_ENABLE_LINES=1

# This is for Azure - it just cannot Chacha20
CFLAGS_MONGOOSE += -DMG_ENABLE_CHACHA20=0

# Windows settings. Assume MinGW compiler.
# To use VS compiler, run: make CC=cl CFLAGS=/MD OUT=/Feprog.exe
ifeq ($(OS),Windows_NT)
  PROG ?= example.exe
  CC = gcc
  CFLAGS += -lws2_32
  DELETE = cmd /C del /Q /F /S
  OUT ?= -o $(PROG)
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

clean:
	$(DELETE) $(PROG) *.o *.obj *.exe *.dSYM mbedtls
