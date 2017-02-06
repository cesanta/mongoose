This is a simple project template.

sample_lib is an example for multi-level folder Makefile, notice the folder structure and each Makefile, you can get the clue.


HOWTO:
1. Copy this folder to anywhere.
Example:
   Copy to ~/workspace/project_template
   You can rename this folder as you like.

2. Export SDK_PATH and BIN_PATH.
Example:
   Your SDK path is ~/esp_iot_rtos_sdk, and want generate bin at ~/esp8266_bin.
   Do follow steps:
     1>. export SDK_PATH=~/esp_iot_rtos_sdk
     2>. export BIN_PATH=~/esp8266_bin
   SDK and project are seperate, you can update SDK without change your project.

3. Enter project_template folder, run ./gen_misc.sh, and follow the tips and steps.


Compile Options:
(1) COMPILE
    Possible value: xcc
    Default value:
    If not set, use gcc by default.

(2) BOOT
    Possible value: none/old/new
      none: no need boot
      old: use boot_v1.1
      new: use boot_v1.2
    Default value: new

(3) APP
    Possible value: 0/1/2
      0: original mode, generate eagle.app.v6.flash.bin and eagle.app.v6.irom0text.bin
      1: generate user1
      2: generate user2
    Default value: 0

(3) SPI_SPEED
    Possible value: 20/26.7/40/80
    Default value: 40

(4) SPI_MODE
    Possible value: QIO/QOUT/DIO/DOUT
    Default value: QIO

(4) SPI_SIZE_MAP
    Possible value: 0/2/3/4/5/6
    Default value: 0

For example:
    make COMPILE=gcc BOOT=new APP=1 SPI_SPEED=40 SPI_MODE=QIO SPI_SIZE_MAP=0
