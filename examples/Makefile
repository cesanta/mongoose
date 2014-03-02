CFLAGS    = -W -Wall -I.. -pthread -g -pipe $(CFLAGS_EXTRA)
RM        = rm -rf

ifeq ($(OS),Windows_NT)
  MSVC      = ../../vc6
  RM = del /q /f
  CC = $(MSVC)/bin/cl $(CLFLAGS)
  CLFLAGS   = /MD /TC /nologo $(CFLAGS_EXTRA) /W3 \
              /I$(MSVC)/include /I.. /Dsnprintf=_snprintf
  LFLAGS    = /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86
else
  UNAME_S := $(shell uname -s)

  ifeq ($(UNAME_S),Linux)
    CFLAGS += -ldl
  endif

  ifeq ($(UNAME_S),Darwin)
  endif
endif

all: hello websocket server post multi_threaded upload auth

# To enable Lua in a server, uncomment following lines
LUA    = ../lua-5.2.3/src
#CFLAGS += -I$(LUA) -L$(LUA) -llua

hello: hello.c ../mongoose.c
	$(CC) hello.c ../mongoose.c  -o hello $(CFLAGS)

websocket: websocket_html.c websocket.c ../mongoose.c
	$(CC) websocket.c websocket_html.c ../mongoose.c  -o websocket $(CFLAGS)

server: server.c ../mongoose.c
	$(CC) server.c ../mongoose.c -o server $(CFLAGS)

post: post.c ../mongoose.c
	$(CC) post.c ../mongoose.c -o post $(CFLAGS)

multi_threaded: multi_threaded.c ../mongoose.c
	$(CC) multi_threaded.c ../mongoose.c -o multi_threaded $(CFLAGS)

upload: upload.c ../mongoose.c
	$(CC) upload.c ../mongoose.c -o upload $(CFLAGS)

auth: auth.c ../mongoose.c
	$(CC) auth.c ../mongoose.c -o auth $(CFLAGS)

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

u:
	g++ unit_test.c -Wall -W -pedantic -lssl -g -O0 && ./a.out


clean:
	-@$(RM) hello upload post websocket auth server *.exe *.dSYM *.obj .*o
