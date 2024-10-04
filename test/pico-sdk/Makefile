PROJECTS ?= $(wildcard ../../examples/rp2040/*-sdk-*)

all: $(PROJECTS)
	echo 

$(PROJECTS): pico-sdk
	(make -C $@ build && make -C $@ clean) || ( \
	ln -s $(PWD)/pico-sdk $@/pico-sdk && \
	cd $@ && rm -rf build && mkdir -p build && \
	cd build && cmake -G "Unix Makefiles" .. && make && \
	rm -rf build pico-sdk )

pico-sdk:
	git clone --depth 1 -b 1.5.1 https://github.com/raspberrypi/pico-sdk $@
	cd $@ && git submodule update --init

clean:
	rm -rf pico-sdk
