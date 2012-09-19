CFLAGS=	-W -Wall -I.. -pthread -g

all:
	OS=`uname`; \
	  test "$$OS" = Linux && LIBS="-ldl" ; \
	  $(CC) $(CFLAGS) hello.c ../mongoose.c  $$LIBS $(ADD) -o hello;
	  $(CC) $(CFLAGS) upload.c ../mongoose.c  $$LIBS $(ADD) -o upload;
	  $(CC) $(CFLAGS) post.c ../mongoose.c  $$LIBS $(ADD) -o post;
	  $(CC) $(CFLAGS) -DUSE_WEBSOCKET websocket.c ../mongoose.c  $$LIBS $(ADD) -o websocket;
	  $(CC) $(CFLAGS) chat.c ../mongoose.c  $$LIBS $(ADD) -o chat
