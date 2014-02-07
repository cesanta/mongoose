CFLAGS    = -W -Wall -I.. -pthread -g -pipe $(CFLAGS_EXTRA)
RM        = rm -rf
MSVC      = ../../vc6
CL        = $(MSVC)/bin/cl
CLFLAGS   = /MD /TC /nologo $(CFLAGS_EXTRA) /W3 \
            /I$(MSVC)/include /I.. /Dsnprintf=_snprintf
LFLAGS    = /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86


ifeq ($(OS),Windows_NT)
  RM = del /q /f
  CC = $(MSVC)/bin/cl $(CLFLAGS)
else
  UNAME_S := $(shell uname -s)
  DLL_FLAGS += -shared

  ifeq ($(UNAME_S),Linux)
    CFLAGS += -ldl
  endif

  ifeq ($(UNAME_S),Darwin)
    #  DLL_FLAGS += -bundle -undefined dynamic_lookup -dynamiclib
    DLL_FLAGS += -flat_namespace -undefined suppress -dynamiclib
  endif
endif

# To enable Lua in a server, uncomment following lines
LUA    = ../lua-5.2.3/src
#CFLAGS += -I$(LUA) -L$(LUA) -llua

all: websocket_html.c
	$(CC) hello.c ../mongoose.c  -o hello $(CFLAGS)
	$(CC) websocket.c websocket_html.c ../mongoose.c  -o websocket $(CFLAGS)
	$(CC) post.c ../mongoose.c -o post $(CFLAGS)
	$(CC) multi_threaded.c ../mongoose.c -o multi_threaded $(CFLAGS)
	$(CC) upload.c ../mongoose.c -o upload $(CFLAGS)
	$(CC) auth.c ../mongoose.c -o auth $(CFLAGS)
	$(CC) server.c ../mongoose.c -o server $(CFLAGS)

websocket_html.c: websocket.html
	perl mkdata.pl $< > $@

windows: websocket_html.c
	$(CL) hello.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) websocket.c websocket_html.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) post.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) multi_threaded.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) upload.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) auth.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) server.c ../mongoose.c $(CLFLAGS) $(LFLAGS)

clean:
	-@$(RM) hello upload post websocket auth server *.exe *.dSYM *.obj .*o
