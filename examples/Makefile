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

all:
	$(CC) hello.c ../mongoose.c  -o hello $(CFLAGS)
	$(CC) qcomm.c ../mongoose.c  -o qcomm $(CFLAGS)
	$(CC) post.c ../mongoose.c -o post $(CFLAGS)
	$(CC) multi_threaded.c ../mongoose.c -o multi_threaded $(CFLAGS)

#	$(CC) upload.c ../mongoose.c -o upload $(CFLAGS)
#	$(CC) -DUSE_WEBSOCKET websocket.c ../mongoose.c -o $@ $(CFLAGS)
#	$(CC) chat.c ../mongoose.c -o chat $(CFLAGS)
#	$(CC) lua_dll.c ../build/lua_5.2.1.c -o $@.so $(CFLAGS) $(DLL_FLAGS)


MSVC    = ../../vc6
CL      = $(MSVC)/bin/cl
CLFLAGS = /MD /TC /nologo $(DBG) /W3 /DNO_SSL \
        /I$(MSVC)/include /I.. /Dsnprintf=_snprintf
LFLAGS  = /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86

windows:
	$(CL) hello.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) post.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
	$(CL) multi_threaded.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
#	$(CL) upload.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
#	$(CL) /DUSE_WEBSOCKET websocket.c ../mongoose.c $(CLFLAGS) $(LFLAGS)
#$(CL) lua_dll.c $(CLFLAGS) $(DLL_FLAGS) /DLL $(LFLAGS) /SUBSYSTEM:WINDOWS /ENTRY:luaopen_lua_dll /EXPORT:luaopen_lua_dll /out:lua_dll.dll

clean:
	rm -rf hello upload post websocket chat *.exe *.dSYM *.obj .*o
