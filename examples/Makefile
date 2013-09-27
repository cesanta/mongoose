CFLAGS  = -W -Wall -I.. -pthread -g -pipe $(COPT)
DLL_FLAGS = -DLUA_COMPAT_ALL -I../build

ifeq ($(OS),Windows_NT)
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

all: hello upload post websocket chat

hello:
	$(CC) hello.c ../mongoose.c  -o $@ $(CFLAGS)

upload:
	$(CC) upload.c ../mongoose.c -o $@ $(CFLAGS)

post:
	$(CC) post.c ../mongoose.c -o $@ $(CFLAGS)

websocket:
	$(CC) -DUSE_WEBSOCKET websocket.c ../mongoose.c -o $@ $(CFLAGS)

chat:
	$(CC) chat.c ../mongoose.c -o $@ $(CFLAGS)

lua_dll:
	$(CC) lua_dll.c ../build/lua_5.2.1.c -o $@.so $(CFLAGS) $(DLL_FLAGS)


MSVC    = ../../vc6
CL      = $(MSVC)/bin/cl
CLFLAGS = /MD /TC /nologo $(DBG) /W3 /DNO_SSL \
        /I$(MSVC)/include /I.. /Dsnprintf=_snprintf
LFLAGS  = /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86

windows:
	$(CL) upload.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) hello.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) upload.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) post.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) post.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) /DUSE_WEBSOCKET websocket.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
#$(CL) lua_dll.c $(CLFLAGS) $(DLL_FLAGS) /DLL $(LFLAGS) /SUBSYSTEM:WINDOWS /ENTRY:luaopen_lua_dll /EXPORT:luaopen_lua_dll /out:lua_dll.dll

clean:
	rm -rf hello upload post websocket chat *.exe *.dSYM *.obj .*o
