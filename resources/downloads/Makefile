TOP ?= ../..
# ST_TARGETS = nucleo_h723zg nucleo_f756zg nucleo_h563zi
ST_TARGETS = $(basename $(notdir $(wildcard $(TOP)/tutorials/stm32/*/cubemx/*.ioc)))
PROJECTS = full
TARGETS = desktop $(ST_TARGETS)
MX ?= /Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX

DIRS = $(foreach P,$(PROJECTS),$(foreach T,$(TARGETS),$(T)_dashboard_$(P)))
ZIPS = $(foreach D,$(DIRS),$(D).zip)

all:
	@echo $(TARGETS)

dirs: $(DIRS)

DESKTOP_DIRS = $(foreach P,$(PROJECTS),desktop_dashboard_$(P))

$(DESKTOP_DIRS): desktop_dashboard_%:
	mkdir -p $@ $@/resources
	$(MAKE) -C $(TOP)/tutorials/device-dashboard/$* clean
	cp $(TOP)/tutorials/device-dashboard/$*/*{c,html,Makefile} $@
	cp $(TOP)/mongoose.{c,h} $@
	cp $(TOP)/LICENSE $@
	cp $(TOP)/resources/*.js $@/resources
	sed -i ""  "s,../../..,.," $@/Makefile

define ST_DASHBOARD_RULE
$(1)_dashboard_$(2): $(TOP)/tutorials/stm32/$(1)/cubemx/$(1).ioc
	mkdir -p $$@
	cp $$^ $$@
	echo config load `realpath $$@/$(1).ioc` > gen.mx
	echo project generate >> gen.mx
	echo exit >> gen.mx
	$(MX) -q `pwd`/gen.mx
	sh integrate_mongoose_cubemx.sh $$@ $(2)
endef

$(foreach T,$(ST_TARGETS), $(foreach P,$(PROJECTS), $(eval $(call ST_DASHBOARD_RULE,$(T),$(P)))))

%.zip: %
	zip -qr $@ $<

upload: $(ZIPS)
	scp $(ZIPS) h1:/data/downloads/

clean:
	rm -rf $(DIRS) *.zip

# rm -rf $(DIRS) $(ZIPS) #$(foreach T,$(ST_TARGETS),$(T)_mongoose)
