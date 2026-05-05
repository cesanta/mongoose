TOP ?= ../..
ST_TARGETS = nucleo_h723zg nucleo_f756zg nucleo_h563zi
#ST_TARGETS = $(subst .ioc,,$(wildcard *.ioc))
PROJECTS = full minimal
TARGETS = desktop $(ST_TARGETS)
MX ?= /Applications/STM32CubeMX.app/Contents/MacOs/STM32CubeMX

DIRS = $(foreach P,$(PROJECTS),$(foreach T,$(TARGETS),$(T)_$(P)))
ZIPS = $(foreach D,$(DIRS),$(D).zip)

all: $(ZIPS)

DESKTOP_DIRS = $(foreach P,$(PROJECTS),desktop_$(P))
#H723_DIRS = $(foreach P,$(PROJECTS),h723_$(P))

$(DESKTOP_DIRS): desktop_%:
	mkdir -p $@ $@/resources
	$(MAKE) -C $(TOP)/tutorials/device-dashboard/$* clean
	cp $(TOP)/tutorials/device-dashboard/$*/*{c,md,html,Makefile} $@
	cp $(TOP)/mongoose.{c,h} $@
	cp $(TOP)/LICENSE $@
	cp $(TOP)/resources/*.js $@/resources
	sed -i ""  "s,../../..,.," $@/Makefile

define ST_RULE
$(1)_$(2): $(1).ioc
	mkdir -p $$@
	cp $(1).ioc $$@
	echo config load `pwd`/$$@/$(1).ioc > gen.mx
	echo project generate >> gen.mx
	echo exit >> gen.mx
	test -d $$@/Core || $(MX) -q `pwd`/gen.mx
	grep mongoose.h $$@/Core/Src/main.c || sed -i '' '/BEGIN Includes/r patch_include.c' $$@/Core/Src/main.c
	grep '^int _write' $$@/Core/Src/main.c || sed -i '' '/USER CODE BEGIN 0 /r patch_write.c' $$@/Core/Src/main.c
	grep 'mg_dash_init' $$@/Core/Src/main.c || sed -i '' '/USER CODE BEGIN WHILE/,/USER CODE END WHILE/{//!d;};' $$@/Core/Src/main.c
	grep 'mg_dash_init' $$@/Core/Src/main.c || sed -i '' '/USER CODE BEGIN WHILE/r patch_loop.c' $$@/Core/Src/main.c
	test -f $$@/STM32H723XG_FLASH.ld && (grep eth_ram $$@/STM32H723XG_FLASH.ld || sed -i '' '/_sidata =/r patch_ld.txt'  $$@/STM32H723XG_FLASH.ld) || true
	$(MAKE) -C $(TOP)/tutorials/device-dashboard/$(2) file_data.c
	cp $(TOP)/tutorials/device-dashboard/$(2)/{file_data.c,dashboard.c} $$@/Middlewares/Third_Party/Cesanta_Mongoose/
	cp $(TOP)/LICENSE $$@
endef

$(foreach T,$(ST_TARGETS), $(foreach P,$(PROJECTS), $(eval $(call ST_RULE,$(T),$(P)))))

%.zip: %
	zip -qr $@ $<

upload: $(ZIPS)
	scp $(ZIPS) h1:/data/downloads/

clean:
	rm -rf $(DIRS) $(ZIPS) gen.mx
