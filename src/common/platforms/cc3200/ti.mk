IPATH += $(SDK_PATH)/third_party/FreeRTOS/source/portable/CCS/ARM_CM3
VPATH += $(SDK_PATH)/third_party/FreeRTOS/source/portable/CCS/ARM_CM3

CC_WRAPPER ?=
CC = $(TOOLCHAIN)/bin/armcl
AR = $(TOOLCHAIN)/bin/armar
NM = nm
GENFILES_LIST ?=

C_CXX_FLAGS = -Dccs -I$(TOOLCHAIN)/include
TI_C_CXX_FLAGS = -mv7M4 --little_endian --code_state=16 --float_support=vfplib --abi=eabi \
                 -O4 --opt_for_speed=0 --unaligned_access=on --small_enum \
                 --gen_func_subsections=on --diag_wrap=off --display_error_number \
                 --emit_warnings_as_errors
CFLAGS += --c99 $(TI_C_CXX_FLAGS) $(C_CXX_FLAGS)
CXXFLAGS += $(TI_C_CXX_FLAGS) $(C_CXX_FLAGS)

# cc flags,file
define cc
	$(vecho) "TICC  $2"
	$(Q) $(CC_WRAPPER) $(CC) -c --preproc_with_compile -ppd=$@.d $1 --output_file=$@ $2
endef
define cxx
	$(vecho) "TICXX $2"
	$(Q) $(CC_WRAPPER) $(CC) -c --preproc_with_compile -ppd=$@.d $1 --output_file=$@ $2
endef

# asm flags,file
define asm
	$(vecho) "TIASM $2"
	$(Q) $(CC_WRAPPER) $(CC) -c $1 --output_file=$@ $2
endef

# ar files
define ar
	$(vecho) "TIAR  $@"
	$(Q) $(AR) qru $@ $1
endef

# link script,flags,objs
define link
	$(vecho) "TILD  $@"
	$(Q) $(CC_WRAPPER) $(CC) \
	  -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi --little_endian \
	  --run_linker \
	  --generate_dead_funcs_list=$@.garbage.xml \
	  -i $(TOOLCHAIN)/lib \
	  --reread_libs --warn_sections --display_error_number \
	  --unused_section_elimination=on \
	  -o $@ --map_file=$@.map --xml_link_info=$@.map.xml \
	  $2 $1 $3
endef
