TOP ?= ../..
# ST_TARGETS = nucleo_h723zg nucleo_f756zg nucleo_h563zi
ST_TARGETS = $(basename $(notdir $(wildcard $(TOP)/tutorials/stm32/*/cubemx/*.ioc)))
PROJECTS = full
TARGETS = desktop pico2_w $(ST_TARGETS)
MX ?= /Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX

DIRS = $(foreach P,$(PROJECTS),$(foreach T,$(TARGETS),$(T)_dashboard_$(P)))
ZIPS = $(foreach D,$(DIRS),$(D).zip)

all:
	@echo $(TARGETS)

dirs: $(DIRS)

zips: $(ZIPS)

DESKTOP_DIRS = $(foreach P,$(PROJECTS),desktop_dashboard_$(P))

$(DESKTOP_DIRS): desktop_dashboard_%:
	mkdir -p $@ $@/resources
	$(MAKE) -C $(TOP)/tutorials/device-dashboard/$* clean
	cp $(TOP)/tutorials/device-dashboard/$*/*{c,html,Makefile} $@
	cp $(TOP)/mongoose.{c,h} $@
	cp $(TOP)/LICENSE $@
	cp $(TOP)/resources/*.js $@/resources
	sed -i ""  "s,../../..,.," $@/Makefile

.PHONY: pico2_w_dashboard_full

pico2_w_dashboard_full pico2_w_dashboard_minimal: pico2_w_dashboard_%:
	mkdir -p $@
	$(MAKE) -C $(TOP)/tutorials/pico-sdk/pico2_w fullclean
	cp $(TOP)/tutorials/pico-sdk/pico2_w/* $@/
	cp $(TOP)/{mongoose.c,mongoose.c,LICENSE} $@/
	cp $(TOP)/resources/html2c.js $@/
	cp $(TOP)/tutorials/device-dashboard/$*/{dashboard.c,dashboard.html} $@/
	sed -i '' -e '/mg_http_listen/d' $@/main.c
	grep -q 'mg_dash_init' $@/main.c || perl -i -ne 'print; print "  mg_dash_init(&mgr);\n" if /mg_mgr_init/' $@/main.c
	grep -q 'mg_dash_poll' $@/main.c || perl -i -ne 'print; print "    mg_dash_poll(&mgr);\n" if /mg_mgr_poll/' $@/main.c
	grep -q 'dashboard.c' $@/CMakeLists.txt || sed -i -e 's,mongoose.c,mongoose.c dashboard.c file_data.c,' $@/CMakeLists.txt
	grep -q 'html2c.js' $@/CMakeLists.txt || perl -i -ne 'print "\nadd_custom_command(\n    OUTPUT \$${CMAKE_SOURCE_DIR}/file_data.c\n    COMMAND node \$${CMAKE_SOURCE_DIR}/html2c.js \$${CMAKE_SOURCE_DIR}/dashboard.html -o \$${CMAKE_SOURCE_DIR}/file_data.c\n    DEPENDS \$${CMAKE_SOURCE_DIR}/dashboard.html\n    VERBATIM\n)\n\n" if /add_executable/; print;' $@/CMakeLists.txt

define ST_DASHBOARD_RULE
$(1)_dashboard_$(2): $(TOP)/tutorials/stm32/$(1)/cubemx/$(1).ioc
	mkdir -p $$@
	cp $(TOP)/tutorials/stm32/$(1)/cubemx/* $$@ 2>/dev/null || true
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
