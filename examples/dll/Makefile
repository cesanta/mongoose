# This Makefile builds Mongoose as a shared library on different architectures.
# To pass a specific build options, use CFLAGS_EXTRA, for example:
# make macos CFLAGS_EXTRA="-DMG_ENABLE_COAP"

CFLAGS = -W -Wall -O2 $(CFLAGS_EXTRA)
SRC = ../../mongoose.c

all:
	@echo "make <unix|windows> CFLAGS_EXTRA='-O2'"

unix:
	$(CC) $(SRC) -shared -o mongoose.so $(CFLAGS)

windows:
	cl $(SRC) /LD advapi32.lib $(CFLAGS_EXTRA)

clean:
	rm -rf *.so *.dll *.obj *.exe
