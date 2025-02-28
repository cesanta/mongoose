ROOT ?= $(realpath $(CURDIR)/../..)
ZEPHYR_DIR ?= $(ROOT)/tutorials/zephyr/zephyrproject
DOCKER ?= docker run --rm -v $(ROOT):$(ROOT) -w $(CURDIR)/wizard
IMAGE ?= zephyrprojectrtos/ci
REVNO ?= --mr v3.7-branch
YQ ?= $(DOCKER) --user="root" mikefarah/yq

all: zephyr $(PROJECTS)
	echo

$(PROJECTS): FORCE
	$(DOCKER) -e ZEPHYR_BASE=$(ZEPHYR_DIR)/zephyr $(IMAGE) west build -b $(ZBNAME) -p auto $(realpath $@)
	rm -rf build

FORCE:

zephyr: $(ZEPHYR_DIR)/modules/hal/cmsis

$(ZEPHYR_DIR)/modules/hal/cmsis: $(ZEPHYR_DIR)/zephyr/west.yml
	$(DOCKER) $(IMAGE) /bin/sh -c 'cd $(ZEPHYR_DIR) && west update'
	touch $(ZEPHYR_DIR)/modules/hal/cmsis

# use '(YQ) -i eval' for in-place minify; this allows proper dependency processing and update. ALWAYS include "cmsis"...
$(ZEPHYR_DIR)/zephyr/west.yml: $(ZEPHYR_DIR)/zephyr/west.yml.orig
	$(YQ) eval '(.manifest.defaults, .manifest.remotes, .manifest.projects[] | select(.name == "cmsis" or .name == "hal_stm32" or .name == "hal_nxp" or .name == "hal_espressif" or .name == "hal_rpi_pico" or .name == "segger" or .name == "mbedtls" or .name == "mcuboot" or .name == "picolibc" | del(.null) ), .manifest.self) as $$i ireduce({};setpath($$i | path; $$i)) | del(.manifest.projects.[].null) | del(..|select(length==0))' $(ZEPHYR_DIR)/zephyr/west.yml.orig > $(ZEPHYR_DIR)/zephyr/west.yml

$(ZEPHYR_DIR)/zephyr/west.yml.orig: FORCE
	test -d $(ZEPHYR_DIR) || \
	( mkdir -p $(ZEPHYR_DIR) && \
	$(DOCKER) $(IMAGE) west init $(REVNO) $(ZEPHYR_DIR) && \
	mv $(ZEPHYR_DIR)/zephyr/west.yml $(ZEPHYR_DIR)/zephyr/west.yml.orig )


# Wizard-style board name --> Zephyr board name; ZBNAME = table[BOARD]
Z_f207 := nucleo_f207zg
Z_f429 := nucleo_f429zi
Z_f746 := nucleo_f746zg
Z_f756 := nucleo_f756zg
Z_f767 := nucleo_f767zi
Z_h563 := nucleo_h563zi
Z_h573 := stm32h573i_dk
Z_h723 := nucleo_h723zg
Z_h735 := stm32h735g_disco
Z_h743 := nucleo_h743zi
Z_h745 := stm32h745i_disco/stm32h745xx/m7
Z_h747 := stm32h747i_disco/stm32h747xx/m7
Z_h753 := nucleo_h753zi
Z_h755 := nucleo_h755zi_q/stm32h755xx/m7
Z_mcxn947 := frdm_mcxn947/mcxn947/cpu0
Z_rt1060 := mimxrt1060_evk
Z_rt1064 := mimxrt1064_evk
Z_evb-pico := w5500_evb_pico
ZBNAME = $(Z_$(BOARD))
