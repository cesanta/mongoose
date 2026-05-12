#!/bin/sh

BOARD=${1:-}
DASH=${2:-}
CURL="curl -fsSL"
API="https://api.github.com/repos/cesanta/mongoose"
RAW="https://raw.githubusercontent.com/cesanta/mongoose/HEAD/"

trap "rm -rf gen.mx" EXIT
fail() { echo $@ >&2 ; exit 1 ; }

# Find CubeMX CLI executable
MX=`find /Applications/STMicroelectronics/ -name STM32CubeMX -type f`

test -z "$MX" && fail "CubeMX is not installed. Install it first"
# test -d "$BOARD"  && fail "Directory $BOARD already exists, remove it first"

# Board name is not given. Show list of boards and exit
if test -z "$BOARD" ; then
  echo "Usage: $0 BOARD [full|minimal]"
  echo "Supported boards:"
  $CURL $API/contents/resources/downloads | perl -nle 'print "  $1" if /"name": "(.+?).ioc"/'
  exit 1
fi

# Create project directory
DIR=${BOARD}_mongoose
DIR2=${BOARD}_dashboard_${DASH}
test -d $DIR || mkdir -p $DIR
test -f "$DIR/$BOARD.ioc" || $CURL $RAW/resources/downloads/$BOARD.ioc -o $DIR/$BOARD.ioc 2>/dev/null
test -f "$DIR/$BOARD.ioc" || fail echo "Usupported board $BOARD"

# Run CubeMX to generate project using a pre-configured .ioc file
echo config load `pwd`/$DIR/$BOARD.ioc > gen.mx
echo project generate >> gen.mx
echo exit >> gen.mx
test -f "$DIR/CMakeLists.txt" || $MX -q `pwd`/gen.mx

# Adopt generated code for Mongoose
MAIN_C=$DIR/Core/Src/main.c
grep -q mongoose.h $MAIN_C || perl -i -ne 'print; print "#include \"mongoose.h\"\n" if /BEGIN Includes/' $MAIN_C
grep -q '^int _write' $MAIN_C || perl -i -ne 'print; print "int _write(int fd, unsigned char *buf, int len) {\n  HAL_UART_Transmit(&huart3, buf, len, HAL_MAX_DELAY);\n  return len;\n}\n" if /USER CODE BEGIN 0/' $MAIN_C
grep -q 'mg_mgr_init' $MAIN_C || perl -i -ne 'print; print "  struct mg_mgr mgr;\n  mg_mgr_init(&mgr);\n\n" if /USER CODE BEGIN WHILE/' $MAIN_C
grep -q 'mg_mgr_poll' $MAIN_C || perl -i -ne 'print "    mg_mgr_poll(&mgr, 0);\n" if /USER CODE END WHILE/; print;' $MAIN_C
test -f $DIR/STM32H723XG_FLASH.ld && (grep -q eth_ram $DIR/STM32H723XG_FLASH.ld || perl -i -ne 'print ; print "\n  /* Mongoose Ethernet driver */\n  .eth_ram : { *(.eth_ram .eth_ram*) } > RAM_D2 AT > FLASH\n" if /_sidata =/' $DIR/STM32H723XG_FLASH.ld)
test -f $DIR/LICENSE || $CURL $RAW/LICENSE -o $DIR/LICENSE

# If dashboard is specified, copy it to the project
if test "$DASH" == "full" -o "$DASH" == "minimal" ; then
  test -d $DIR2 || cp -r $DIR $DIR2
  test -d $DIR2/Mongoose || mkdir -p $DIR2/Mongoose
  test -f $DIR2/Mongoose/dashboard.c || $CURL $RAW/tutorials/device-dashboard/$DASH/dashboard.c -o $DIR2/Mongoose/dashboard.c
  test -f $DIR2/Mongoose/dashboard.html || $CURL $RAW/tutorials/device-dashboard/$DASH/dashboard.html -o $DIR2/Mongoose/dashboard.html
  test -f $DIR2/Mongoose/html2c.js || $CURL $RAW/resources/html2c.js -o $DIR2/Mongoose/html2c.js
  MAIN2_C=$DIR2/Core/Src/main.c
  grep -q 'mg_dash_init' $MAIN2_C || perl -i -ne 'print; print "  mg_dash_init(&mgr);\n" if /mg_mgr_init/' $MAIN2_C
  grep -q 'mg_dash_poll' $MAIN2_C || perl -i -ne 'print; print "    mg_dash_poll(&mgr);\n" if /mg_mgr_poll/' $MAIN2_C
  grep -q '^    Mongoose/file_data.c' $DIR2/CMakeLists.txt || perl -i -ne 'print; print "    Mongoose/dashboard.c\n    Mongoose/file_data.c\n" if /# Add user sources here/' $DIR2/CMakeLists.txt
  grep -q 'html2c.js' $DIR2/CMakeLists.txt || perl -i -ne 'print "# Generate file_data.c from dashboard.html\nadd_custom_command(\n    OUTPUT \${CMAKE_SOURCE_DIR}/Mongoose/file_data.c\n    COMMAND node \${CMAKE_SOURCE_DIR}/Mongoose/html2c.js \${CMAKE_SOURCE_DIR}/Mongoose/dashboard.html -o \${CMAKE_SOURCE_DIR}/Mongoose/file_data.c\n    DEPENDS \${CMAKE_SOURCE_DIR}/Mongoose/dashboard.html\n    VERBATIM\n)\n\n" if /^# Add sources to executable/; print;' $DIR2/CMakeLists.txt
fi
