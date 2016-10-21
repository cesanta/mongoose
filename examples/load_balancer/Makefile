# To build with SSL under windows, do:
# wine make load_balancer.exe SSL=openssl  # OpenSSL build
# wine make load_balancer.exe SSL=krypton  # Krypton build

PROG = load_balancer
SOURCES = $(PROG).c ../../mongoose.c
CFLAGS = -W -Wall -pthread $(CFLAGS_EXTRA)

ifeq ($(SSL), openssl)
  OPENSSL_PATH = ./openssl-0.9.8
  CFLAGS_EXTRA += -DMG_ENABLE_SSL -I$(OPENSSL_PATH)/include
	CFLAGS_EXTRA += /link /libpath:$(OPENSSL_PATH)/lib ssleay32.lib libeay32.lib
endif

ifeq ($(SSL), krypton)
	KRYPTON_PATH = ../../../krypton
  CFLAGS_EXTRA += -DMG_ENABLE_SSL $(KRYPTON_PATH)/krypton.c -I$(KRYPTON_PATH)
endif

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I.. /MD /Fe$@ /DMG_ENABLE_THREADS advapi32.lib $(CFLAGS_EXTRA)

test: $(PROG)
	$(MAKE) -C ../api_server
	sh unit_test.sh $$(pwd)/$(PROG)

docker-build:
	docker build -t cesanta/load_balancer .

docker-push:
	docker push cesanta/load_balancer

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
