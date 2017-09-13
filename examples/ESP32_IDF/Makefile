SDK_VER=$(shell cat sdk.version)

.PHONY: all

MG_PATH = $(realpath $(PWD)/../../)

all:
	docker run --rm -it -v $(MG_PATH):/esp32_idf \
                   $(SDK_VER) \
                   bash -c "cd esp32_idf/examples/ESP32_IDF && \
                   make -f Makefile.build defconfig && make -f Makefile.build"

clean:
	docker run --rm -it -v $(MG_PATH):/esp32_idf \
                   $(SDK_VER) \
                   bash -c "rm -rf esp32_idf/examples/ESP32_IDF/build"
