#!/bin/sh

# IOC="${1:-}"
DIR="${1:-}"
ROOT_DIR=$DIR
DASH=${2:-}
CURL="curl -fsSL"
RAW="https://raw.githubusercontent.com/cesanta/mongoose/HEAD/"

trap "rm -rf gen.mx" EXIT
fail() { echo $@ >&2 ; exit 1 ; }

test -d "$DIR" || fail "Usage: $0 DIRECTORY"

test -f $DIR/FSBL/Core/Src/main.c && DIR=$DIR/FSBL
test -f $DIR/CM7/Core/Src/main.c && DIR=$DIR/CM7

# If we're running from the mongoose repo, cp instead of curl from github
TOP=$(realpath $(dirname "$0")/../..)
local_curl() { cp "$TOP/${1#$RAW}" "$3"; }
test -f "$TOP/mongoose.c" && CURL=local_curl

# Add mongoose core files
test -d $DIR/Mongoose || mkdir -p $DIR/Mongoose
test -f $DIR/Mongoose/mongoose.c || $CURL $RAW/mongoose.c -o $DIR/Mongoose/mongoose.c
test -f $DIR/Mongoose/mongoose.h || $CURL $RAW/mongoose.h -o $DIR/Mongoose/mongoose.h
test -f $DIR/Mongoose/mongoose_config.h || (echo "#pragma once" ; echo "#define MG_ARCH MG_ARCH_CUBE" ; ) >> $DIR/Mongoose/mongoose_config.h
case $DIR in *_n657*) (grep -q MG_TLS_NONE $DIR/Mongoose/mongoose_config.h || echo "#define MG_TLS MG_TLS_NONE" >> $DIR/Mongoose/mongoose_config.h) ;; esac

# Patch main.c and linker script
MAIN_C=$DIR/Core/Src/main.c
patch_linker_script() {
  LD=$1
  test -f "$LD" || return
  echo $LD
  grep -q RAM_D2 "$LD" || perl -i -ne 'print; print "  RAM_D2 (xrw)   : ORIGIN = 0x24000000, LENGTH =  512K\n" if /^\s*MEMORY\b.*\{/ || ($m && /\{/); $m = /^\s*MEMORY\b/ && !/\{/' "$LD"
  grep -q eth_ram "$LD" || perl -i -ne 'print ; print "\n  /* Mongoose Ethernet driver */\n  .eth_ram : { *(.eth_ram .eth_ram*) } > RAM_D2 AT > FLASH\n" if /_sidata =/' "$LD"
}
HUART=`perl -nle 'print \$1 if /^UART_HandleTypeDef (.+);/' $MAIN_C`
grep -q mongoose.h $MAIN_C || perl -i -ne 'print; print "#include \"mongoose.h\"\n" if /BEGIN Includes/' $MAIN_C
grep -q '^int _write' $MAIN_C || perl -i -ne "print; print \"int _write(int fd, unsigned char *buf, int len) {\n  HAL_UART_Transmit(&${HUART}, buf, len, HAL_MAX_DELAY);\n  return len;\n}\n\" if /USER CODE BEGIN 0/" $MAIN_C
grep -q 'mg_mgr_init' $MAIN_C || perl -i -ne 'print; print "  struct mg_mgr mgr;\n  mg_mgr_init(&mgr);\n\n" if /USER CODE BEGIN WHILE/' $MAIN_C
grep -q 'mg_mgr_poll' $MAIN_C || perl -i -ne 'print "    mg_mgr_poll(&mgr, 0);\n" if /USER CODE END WHILE/; print;' $MAIN_C
patch_linker_script $DIR/STM32H723XG_FLASH.ld
patch_linker_script $DIR/STM32H743XX_FLASH.ld
patch_linker_script $DIR/STM32H743ZITX_FLASH.ld
patch_linker_script $DIR/STM32H747XIHX_FLASH.ld
patch_linker_script $DIR/stm32h747xx_flash_CM7.ld
test -f $DIR/LICENSE || $CURL $RAW/LICENSE -o $DIR/LICENSE
test -f $DIR/AGENTS.md || $CURL $RAW/AGENTS.md -o $DIR/AGENTS.md
case $DIR in portenta*) (
  grep -q 'void hwspecific_sdio_init(void);' "$MAIN_C" || \
    perl -i -ne 'print; print "void hwspecific_sdio_init(void);\n" if /USER CODE BEGIN 0/' "$MAIN_C"
  grep -q 'hwspecific_sdio_init();' "$MAIN_C" || \
    perl -i -ne 'print "  hwspecific_sdio_init();\n" if /mg_mgr_init\(&mgr\);/; print;' "$MAIN_C"
) ;; esac
case $DIR in *u5a5*) (
  grep -q 'void hwspecific_spi_init(void);' "$MAIN_C" || \
    perl -i -ne 'print; print "void hwspecific_spi_init(void);\n" if /USER CODE BEGIN 0/' "$MAIN_C"
  grep -q 'hwspecific_spi_init();' "$MAIN_C" || \
    perl -i -ne 'print "  hwspecific_spi_init();\n" if /mg_mgr_init\(&mgr\);/; print;' "$MAIN_C"
) ;; esac

# Add Mongoose dir to the build
test -f $DIR/CMakeLists.txt && (grep -q 'Mongoose/$' $DIR/CMakeLists.txt || perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}/Mongoose/\n" if /Add user defined include paths/' $DIR/CMakeLists.txt)
test -f $DIR/CMakeLists.txt && (grep -q 'mongoose.c$' $DIR/CMakeLists.txt || perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}/Mongoose/mongoose.c\n" if /Add user sources here/' $DIR/CMakeLists.txt)
case $DIR in portenta*) (
  test -f $ROOT_DIR/mongoose_config.h && mv $ROOT_DIR/mongoose_config.h $DIR/Mongoose
  test -f $ROOT_DIR/wifi.c && mv $ROOT_DIR/wifi.c $DIR/Mongoose
  test -f $ROOT_DIR/getfw.cmake && mv $ROOT_DIR/getfw.cmake $DIR/
  test -f $DIR/CMakeLists.txt && (grep -q 'Mongoose/wifi.c$' "$DIR/CMakeLists.txt" || \
    perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}/Mongoose/wifi.c\n" if /Add user sources here/' "$DIR/CMakeLists.txt")
  test -f $DIR/CMakeLists.txt && (grep -q '^[[:space:]]*\${CMAKE_SOURCE_DIR}[[:space:]]*$' "$DIR/CMakeLists.txt" || \
    perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}\n" if /Add user defined include paths/' "$DIR/CMakeLists.txt")
  test -f $DIR/CMakeLists.txt && (grep -q 'COMPONENT_WHD$' "$DIR/CMakeLists.txt" || \
    perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}/mbed/targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD\n" if /Add user defined include paths/' "$DIR/CMakeLists.txt")
  test -f $DIR/CMakeLists.txt && (grep -q '^add_custom_target(getfw' "$DIR/CMakeLists.txt" || \
    cat >> "$DIR/CMakeLists.txt" <<'EOF'

add_custom_target(getfw
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/getfw.cmake
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_dependencies(${CMAKE_PROJECT_NAME} getfw)
EOF
  )
) ;; esac
case $DIR in *u5a5*) (
  test -f $ROOT_DIR/mongoose_config.h && mv $ROOT_DIR/mongoose_config.h $DIR/Mongoose
  test -f $ROOT_DIR/wifi.c && mv $ROOT_DIR/wifi.c $DIR/Mongoose
  test -f $DIR/CMakeLists.txt && (grep -q 'Mongoose/wifi.c$' "$DIR/CMakeLists.txt" || \
    perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}/Mongoose/wifi.c\n" if /Add user sources here/' "$DIR/CMakeLists.txt")

) ;; esac

# If dashboard is specified, copy it to the project
#if test "$DASH" = "full" -o "$DASH" = "minimal" ; then
if test "$DASH" == "full" -o "$DASH" == "minimal" ; then
  test -f $DIR/Mongoose/dashboard.c || $CURL $RAW/tutorials/device-dashboard/$DASH/dashboard.c -o $DIR/Mongoose/dashboard.c
  test -f $DIR/Mongoose/dashboard.html || $CURL $RAW/tutorials/device-dashboard/$DASH/dashboard.html -o $DIR/Mongoose/dashboard.html
  test -f $DIR/Mongoose/html2c.js || $CURL $RAW/resources/html2c.js -o $DIR/Mongoose/html2c.js
  grep -q 'mg_dash_init' $MAIN_C || perl -i -ne 'print; print "  mg_dash_init(&mgr);\n" if /mg_mgr_init/' $MAIN_C
  grep -q 'mg_dash_poll' $MAIN_C || perl -i -ne 'print; print "    mg_dash_poll(&mgr);\n" if /mg_mgr_poll/' $MAIN_C
  test -f $DIR/CMakeLists.txt && (grep -q 'file_data.c$' $DIR/CMakeLists.txt || perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}/Mongoose/dashboard.c\n    \${CMAKE_SOURCE_DIR}/Mongoose/file_data.c\n" if /# Add user sources here/' $DIR/CMakeLists.txt)
  test -f $DIR/CMakeLists.txt && (grep -q 'html2c.js' $DIR/CMakeLists.txt || perl -i -ne 'print "# Generate file_data.c from dashboard.html\nadd_custom_command(\n    OUTPUT \${CMAKE_SOURCE_DIR}/Mongoose/file_data.c\n    COMMAND node \${CMAKE_SOURCE_DIR}/Mongoose/html2c.js \${CMAKE_SOURCE_DIR}/Mongoose/dashboard.html -o \${CMAKE_SOURCE_DIR}/Mongoose/file_data.c\n    DEPENDS \${CMAKE_SOURCE_DIR}/Mongoose/dashboard.html\n    VERBATIM\n)\n\n" if /^# Add sources to executable/; print;' $DIR/CMakeLists.txt)
fi
