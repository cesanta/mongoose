# Generate pack using current sources

GENPACK ?= ./gen_pack.sh

all:
	rm -rf Boards/*/*/*/Listings      # remove any leftovers, just in case, harmless
	rm -rf Boards/*/*/*/Objects
	rm -rf Boards/*/*/*/DebugConfig
	cp ../../LICENSE License.txt
	cp ../../README.md .
	cp ../../mongoose.[ch] Mongoose/
	grep -l device_dashboard_fn Boards/*/*/*/main.c | xargs -n 1 dirname | sed 's.$$./.' | xargs -n 1 cp ../../examples/device-dashboard/net.c
	grep -l device_dashboard_fn Boards/*/*/*/main.c | xargs -n 1 dirname | sed 's.$$./.' | xargs -n 1 cp ../../examples/device-dashboard/packed_fs.c
	$(GENPACK)
	rm License.txt README.md Mongoose/mongoose.[ch]
	rm -f Boards/*/*/*/net.c
	rm -f Boards/*/*/*/packed_fs.c

clean:
	rm -rf output

# Requirements:
# zip curl cmake ninja-build libxml2-utils patch p7zip-full
# overwrite cmake with cmake-3.25.2-linux-x86_64 or newer
# get cmsis-toolbox-linux-amd64.tar.gz
# CMSIS_PACK_ROOT=/path/to/packrepo
# CMSIS_COMPILER_ROOT=/path/to/cmsis-toolbox-linux-amd64/etc
# PATH=/opt/cmsis-toolbox-linux-amd64/bin:$PATH
# GCC_TOOLCHAIN_12_2_1=/path/to/toolchain/bin

# First time: packrepo will be empty --> cpackget init https://www.keil.com/pack/index.pidx
# Then: ./gen_pack.sh


# Test examples with updated pack
# Keil: run in Windows after installing new pack release
KEILPATH = Boards/*/*/*-keil-*
KEILPROJECTS = $(wildcard $(KEILPATH))
KEILDIRS = $(wildcard $(KEILPATH)/)

keilexamples: $(KEILDIRS)
	$(MAKE) -f ../../test/keil/Makefile "PROJECTS=$(KEILPROJECTS)"
	$(MAKE) -f ../../test/keil/Clean "PROJECTS=$(KEILPROJECTS)"

$(KEILDIRS): FORCE  # copy anyway, even if not needed
	copy /Y ..\..\examples\device-dashboard\net.c $(subst /,\,$@)
	copy /Y ..\..\examples\device-dashboard\packed_fs.c $(subst /,\,$@)
# not removed

FORCE:
