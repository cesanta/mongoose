TOP ?= ../..
ST_TARGETS = nucleo_h723zg nucleo_f756zg nucleo_h563zi
# ST_TARGETS = $(subst .ioc,,$(wildcard *.ioc))
PROJECTS = full minimal
TARGETS = desktop $(ST_TARGETS)
MX ?= /Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX

DIRS = $(foreach P,$(PROJECTS),$(foreach T,$(TARGETS),$(T)_$(P)))
ZIPS = $(foreach D,$(DIRS),$(D).zip)

all: $(ZIPS)

DESKTOP_DIRS = $(foreach P,$(PROJECTS),desktop_$(P))
#H723_DIRS = $(foreach P,$(PROJECTS),h723_$(P))

$(DESKTOP_DIRS): desktop_%:
	mkdir -p $@ $@/resources
	$(MAKE) -C $(TOP)/tutorials/device-dashboard/$* clean
	cp $(TOP)/tutorials/device-dashboard/$*/*{c,html,Makefile} $@
	cp $(TOP)/mongoose.{c,h} $@
	cp $(TOP)/LICENSE $@
	cp $(TOP)/resources/*.js $@/resources
	sed -i ""  "s,../../..,.," $@/Makefile

define ST_RULE
$(1): $(1).ioc    # This creates MX-generated baseline project
	mkdir -p $$@
	cp $(1).ioc $$@
	sh init.sh $(1)

$(1)_$(2): $(1)
	sh init.sh $(1) $(2)
endef

$(foreach T,$(ST_TARGETS), $(foreach P,$(PROJECTS), $(eval $(call ST_RULE,$(T),$(P)))))

%.zip: %
	zip -qr $@ $<

upload: $(ZIPS)
	scp $(ZIPS) h1:/data/downloads/

clean:
	rm -rf $(DIRS) $(ST_TARGETS) $(ZIPS) gen.mx
