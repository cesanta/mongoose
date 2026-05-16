#!/bin/sh

# IOC="${1:-}"
DIR="${1:-}"
DASH=${2:-}
CURL="curl -fsSL"
RAW="https://raw.githubusercontent.com/cesanta/mongoose/HEAD/"

trap "rm -rf gen.mx" EXIT
fail() { echo $@ >&2 ; exit 1 ; }

test -d "$DIR" || fail "Usage: $0 DIRECTORY"

test -f $DIR/FSBL/Core/Src/main.c && DIR=$DIR/FSBL

# Add mongoose core files
test -d $DIR/Mongoose || mkdir -p $DIR/Mongoose
test -f $DIR/Mongoose/mongoose.c || $CURL $RAW/mongoose.c -o $DIR/Mongoose/mongoose.c
test -f $DIR/Mongoose/mongoose.h || $CURL $RAW/mongoose.h -o $DIR/Mongoose/mongoose.h
test -f $DIR/Mongoose/mongoose_config.h || (echo "#pragma once" ; echo "#define MG_ARCH MG_ARCH_CUBE" ; ) >> $DIR/Mongoose/mongoose_config.h
case $DIR in *_n657*) (grep -q MG_TLS_NONE $DIR/Mongoose/mongoose_config.h || echo "#define MG_TLS MG_TLS_NONE" >> $DIR/Mongoose/mongoose_config.h) ;; esac

# Patch main.c and linker script
MAIN_C=$DIR/Core/Src/main.c
HUART=`perl -nle 'print \$1 if /^UART_HandleTypeDef (.+);/' $MAIN_C`
grep -q mongoose.h $MAIN_C || perl -i -ne 'print; print "#include \"mongoose.h\"\n" if /BEGIN Includes/' $MAIN_C
grep -q '^int _write' $MAIN_C || perl -i -ne "print; print \"int _write(int fd, unsigned char *buf, int len) {\n  HAL_UART_Transmit(&${HUART}, buf, len, HAL_MAX_DELAY);\n  return len;\n}\n\" if /USER CODE BEGIN 0/" $MAIN_C
grep -q 'mg_mgr_init' $MAIN_C || perl -i -ne 'print; print "  struct mg_mgr mgr;\n  mg_mgr_init(&mgr);\n\n" if /USER CODE BEGIN WHILE/' $MAIN_C
grep -q 'mg_mgr_poll' $MAIN_C || perl -i -ne 'print "    mg_mgr_poll(&mgr, 0);\n" if /USER CODE END WHILE/; print;' $MAIN_C
test -f $DIR/STM32H723XG_FLASH.ld && (grep -q eth_ram $DIR/STM32H723XG_FLASH.ld || perl -i -ne 'print ; print "\n  /* Mongoose Ethernet driver */\n  .eth_ram : { *(.eth_ram .eth_ram*) } > RAM_D2 AT > FLASH\n" if /_sidata =/' $DIR/STM32H723XG_FLASH.ld)
test -f $DIR/LICENSE || $CURL $RAW/LICENSE -o $DIR/LICENSE

# Add Mongoose dir to the build
test -f $DIR/CMakeLists.txt && (grep -q 'Mongoose/$' $DIR/CMakeLists.txt || perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}/Mongoose/\n" if /Add user defined include paths/' $DIR/CMakeLists.txt)
test -f $DIR/CMakeLists.txt && (grep -q 'mongoose.c$' $DIR/CMakeLists.txt || perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}/Mongoose/mongoose.c\n" if /Add user sources here/' $DIR/CMakeLists.txt)

# If dashboard is specified, copy it to the project
if test "$DASH" == "full" -o "$DASH" == "minimal" ; then
  test -f $DIR/Mongoose/dashboard.c || $CURL $RAW/tutorials/device-dashboard/$DASH/dashboard.c -o $DIR/Mongoose/dashboard.c
  test -f $DIR/Mongoose/dashboard.html || $CURL $RAW/tutorials/device-dashboard/$DASH/dashboard.html -o $DIR/Mongoose/dashboard.html
  test -f $DIR/Mongoose/html2c.js || $CURL $RAW/resources/html2c.js -o $DIR/Mongoose/html2c.js
  grep -q 'mg_dash_init' $MAIN_C || perl -i -ne 'print; print "  mg_dash_init(&mgr);\n" if /mg_mgr_init/' $MAIN_C
  grep -q 'mg_dash_poll' $MAIN_C || perl -i -ne 'print; print "    mg_dash_poll(&mgr);\n" if /mg_mgr_poll/' $MAIN_C
  test -f $DIR/CMakeLists.txt && (grep -q 'file_data.c$' $DIR/CMakeLists.txt || perl -i -ne 'print; print "    \${CMAKE_SOURCE_DIR}/Mongoose/dashboard.c\n    \${CMAKE_SOURCE_DIR}/Mongoose/file_data.c\n" if /# Add user sources here/' $DIR/CMakeLists.txt)
  test -f $DIR/CMakeLists.txt && (grep -q 'html2c.js' $DIR/CMakeLists.txt || perl -i -ne 'print "# Generate file_data.c from dashboard.html\nadd_custom_command(\n    OUTPUT \${CMAKE_SOURCE_DIR}/Mongoose/file_data.c\n    COMMAND node \${CMAKE_SOURCE_DIR}/Mongoose/html2c.js \${CMAKE_SOURCE_DIR}/Mongoose/dashboard.html -o \${CMAKE_SOURCE_DIR}/Mongoose/file_data.c\n    DEPENDS \${CMAKE_SOURCE_DIR}/Mongoose/dashboard.html\n    VERBATIM\n)\n\n" if /^# Add sources to executable/; print;' $DIR/CMakeLists.txt)
fi
