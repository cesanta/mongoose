TOP ?= ../..
# ST_TARGETS = nucleo_h723zg nucleo_f756zg nucleo_h563zi
ST_TARGETS = $(basename $(notdir $(wildcard $(TOP)/tutorials/stm32/*/cubemx/*.ioc)))
RP_TARGETS = pico-w pico-rndis w5500-evb-pico w55rp20-evb-pico
PROJECTS = full
TARGETS = desktop $(RP_TARGETS) $(ST_TARGETS)
MX ?= /Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX

DIRS = $(foreach P,$(PROJECTS),$(foreach T,$(TARGETS),$(T)-dashboard-$(P)))
ZIPS = $(foreach D,$(DIRS),$(D).zip)

all:
	@echo $(TARGETS)

dirs: $(DIRS)

zips: $(ZIPS)

DESKTOP_DIRS = $(foreach P,$(PROJECTS),desktop-dashboard-$(P))

$(DESKTOP_DIRS): desktop-dashboard-%:
	mkdir -p $@ $@/resources
	$(MAKE) -C $(TOP)/tutorials/device-dashboard/$* clean
	cp $(TOP)/tutorials/device-dashboard/$*/*{c,html,Makefile} $@
	cp $(TOP)/mongoose.{c,h} $@
	cp $(TOP)/LICENSE $@
	cp $(TOP)/resources/*.js $@/resources
	sed -i ""  "s,../../..,.," $@/Makefile

define RP_DASHBOARD_RULE
$(1)-dashboard-$(2):
	mkdir -p $$@
	$(MAKE) -C $(TOP)/tutorials/rp/$(1) clean
	cp -r $(TOP)/tutorials/rp/$(1)/* $$@/
	cp $(TOP)/{mongoose.c,mongoose.c,LICENSE} $$@/mongoose/
	cp $(TOP)/resources/html2c.js $$@/mongoose/
	cp $(TOP)/tutorials/device-dashboard/$(2)/{dashboard.c,dashboard.html} $$@/mongoose/
	sed -i '' -e '/mg_http_listen/d' $$@/main.c
	grep -q 'mg_dash_init' $$@/main.c || perl -i -ne 'print; print "  mg_dash_init(&mgr);\n" if /mg_mgr_init/' $$@/main.c
	grep -q 'mg_dash_poll' $$@/main.c || perl -i -ne 'print; print "    mg_dash_poll(&mgr);\n" if /mg_mgr_poll/' $$@/main.c
	grep -q 'dashboard.c' $$@/CMakeLists.txt || sed -i -e 's,mongoose.c,mongoose.c mongoose/dashboard.c mongoose/file_data.c,' $$@/CMakeLists.txt
	grep -q 'html2c.js' $$@/CMakeLists.txt || perl -i -ne 'print "add_custom_command(\n    OUTPUT \$$$${CMAKE_SOURCE_DIR}/mongoose/file_data.c\n    COMMAND node \$$$${CMAKE_SOURCE_DIR}/mongoose/html2c.js \$$$${CMAKE_SOURCE_DIR}/mongoose/dashboard.html -o \$$$${CMAKE_SOURCE_DIR}/mongoose/file_data.c\n    DEPENDS \$$$${CMAKE_SOURCE_DIR}/mongoose/dashboard.html\n    VERBATIM\n)\n\n" if /add_executable/; print;' $$@/CMakeLists.txt
endef

define ST_DASHBOARD_RULE
$(1)-dashboard-$(2): $(TOP)/tutorials/stm32/$(1)/cubemx/$(1).ioc
	mkdir -p $$@
	cp $(TOP)/tutorials/stm32/$(1)/cubemx/* $$@ 2>/dev/null || true
	echo config load `realpath $$@/$(1).ioc` > gen.mx
	echo project generate >> gen.mx
	echo exit >> gen.mx
	$(MX) -q `pwd`/gen.mx
	sh integrate_mongoose_cubemx.sh $$@ $(2)
endef

$(foreach T,$(ST_TARGETS), $(foreach P,$(PROJECTS), $(eval $(call ST_DASHBOARD_RULE,$(T),$(P)))))
$(foreach T,$(RP_TARGETS), $(foreach P,$(PROJECTS), $(eval $(call RP_DASHBOARD_RULE,$(T),$(P)))))

%.zip: %
	zip -qr $@ $<

upload: $(ZIPS)
	scp $(ZIPS) h1:/data/downloads/

clean:
	rm -rf $(DIRS) *.zip
