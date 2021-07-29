THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../..)

all: example

example:
	true

build: src/main/main.c src/main/wifi.c Makefile
	docker run --rm -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) docker.io/mdashnet/8266 make -C src defconfig app

clean:
	rm -rf build
