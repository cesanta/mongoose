CFLAGS = -g -I.  # This is a known version, no need to check
CFLAGS_MONGOOSE += -DMG_ENABLE_LINES=1 -DMG_ENABLE_IPV6=1
CFLAGS_EXTRA ?= -DMG_TLS=MG_TLS_BUILTIN

server: main.c patched_mongoose.c mongoose.h Makefile
	$(CC) main.c patched_mongoose.c $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) -o $@

clean:
	rm -f server
