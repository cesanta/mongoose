rBoot - An open source boot loader for the ESP8266
--------------------------------------------------
by Richard A Burton, richardaburton@gmail.com
http://richard.burtons.org/


rBoot is designed to be a flexible open source boot loader, a replacement for
the binary blob supplied with the SDK. It has the following advantages over the
Espressif loader:

  - Open source (written in C).
  - Supports up to 256 roms.
  - Roms can be variable size.
  - Able to test multiple roms to find a valid backup (without resetting).
  - Flash layout can be changed on the fly (with care and appropriately linked
    rom images).
  - GPIO support for rom selection.
  - Wastes no stack space (SDK boot loader uses 144 bytes).
  - Documented config structure to allow easy editing from user code.
  - Can validate .irom0.text section with checksum.

Limitations
-----------
The ESP8266 can only map 8Mbits (1MB) of flash to memory, but which 8Mbits to
map is selectable. This allows individual roms to be up to 1MB in size, so long
as they do not straddle an 8Mbit boundary on the flash. This means you could
have four 1MB roms or 8 512KB roms on a 32Mbit flash (such as on the ESP-12), or
a combination. Note, however, that you could not have, for example, a 512KB rom
followed immediately by a 1MB rom because the 2nd rom would then straddle an
8MBit boundary. By default support for using more than the first 8Mbit of the
flash is disabled, because it requires several steps to get it working. See
below for instructions.

Building
--------
A Makefile is included, which should work with the gcc xtensa cross compiler.
There are two source files, the first is compiled and included as data in the
second. When run this code is copied to memory and executed (there is a good
reason for this, see my blog for an explanation). The make file will handle this
for you, but you'll need my esptool2 (see github).

To use the Makefile set SDK_BASE to point to the root of the Espressif SDK and
either set XTENSA_BINDIR to the gcc xtensa bin directory or include it in your
PATH. These can be set as environment variables or by editing the Makefile.

Two small assembler stub functions allow the bootloader to launch the user code
without reserving any space on the stack (while the SDK boot loader uses 144
bytes). This compiles fine with GCC, but if you use another compiler and it
will not compile/work for you then uncomment the #define BOOT_NO_ASM in rboot.h
to use a C version of these functions (this uses 32 bytes).

Tested with SDK v1.3 and GCC v4.8.2.

Installation
------------
Simply write rboot.bin to the first sector of the flash. Remember to set your
flash size correctly with your chosen flash tool (e.g. for esptool.py use the
-fs option). When run rBoot will create it's own config at the start of sector
two for a simple two rom system. You can can then write your two roms to flash
addresses 0x2000 and (half chip size + 0x2000). E.g. for 8Mbit flash:
  esptool.py -fs 8m 0x0000 rboot.bin 0x2000 user1.bin 0x82000 user2.bin

Note: your device may need other options specified. E.g. The nodemcu devkit v1.0
(commonly, but incorrectly, sold as v2) also needs the "-fm dio" option.

For more interesting rom layouts you'll need to write an rBoot config sector
manually, see next step.

The two testload bin files can be flashed in place of normal user roms for
testing rBoot. You do not need these for normal use.

rBoot Config
------------
typedef struct {
	uint8 magic;           // our magic
	uint8 version;         // config struct version
	uint8 mode;            // boot loader mode
	uint8 current_rom;     // currently selected rom
	uint8 gpio_rom;        // rom to use for gpio boot
	uint8 count;           // number of roms in use
	uint8 unused[2];       // padding
	uint32 roms[MAX_ROMS]; // flash addresses of the roms
#ifdef BOOT_CONFIG_CHKSUM
	uint8 chksum;          // boot config chksum
#endif
} rboot_config;

Write a config structure as above to address 0x1000 on the flash. If you want
more than 4 roms (default) just increase MAX_ROMS when you compile rBoot.
Think about how you intend to layout your flash before you start!
Rom addresses must be sector aligned i.e start on a multiple of 4096.

  - magic should have value 0xe1 (defined as BOOT_CONFIG_MAGIC).
  - version is used in case the config structure changes after deployment. It is
    defined as 0x01 (BOOT_CONFIG_VERSION). I don't intend to increase this, but
    you should if you choose to reflash the bootloader after deployment and
    the config structure has changed.
  - mode can be 0x00 (MODE_STANDARD) or 0x01 (MODE_GPIO_ROM). If you set GPIO
    you will need to set gpio_rom as well. The sample GPIO code uses GPIO 16 on
    a nodemcu dev board, if you want to use a different GPIO you'll need to
    adapt the code in rBoot slightly.
  - current_rom is the rom to boot, numbered 0 to count-1.
  - gpio_rom is the rom to boot when the GPIO is triggered at boot.
  - count is the number of roms available (may be less than MAX_ROMS, but not
    more).
  - unused[2] is padding so the uint32 rom addresses are 4 bytes aligned.
  - roms is the array of flash address for the roms. The default generated
    config will contain two entries: 0x00002000 and 0x00082000.
  - chksum (if enabled, not by deafult) should be the xor of 0xef followed by
    each of the bytes of the config structure up to (but obviously not
    including) the chksum byte itself.

Linking user code
-----------------
Each rom will need to be linked with an appropriate linker file, specifying
where it will reside on the flash. If you are only flashing one rom to multiple
places on the flash it must be linked multiple times to produce the set of rom
images. This is the same as with the SDK loader.

Because there are endless possibilities for layout with this loader I don't
supply sample linker files. Instead I'll tell you how to make them.

For each rom slot on the flash take a copy of the eagle.app.v6.ld linker script
from the sdk. You then need to modify just one line in it for each rom:
  irom0_0_seg :                         org = 0x40240000, len = 0x3C000

Change the org address to be 0x40200000 (base memory mapped location of the
flash) + flash address + 0x10 (offset of data after the header).
The logical place for your first rom is the third sector, address 0x2000.
  0x40200000 + 0x2000 + 0x10 = 0x40202010
If you use the default generated config the loader will expect to find the
second rom at flash address half-chip-size + 0x2000 (e.g. 0x82000 on an 8MBit
flash) so the irom0_0_seg should be:
  0x40200000 + 0x82000 + 0x10 = 0x40282010
Due to the limitation of mapped flash (max 8MBit) if you use a larger chip and
do not have big flash support enabled the second rom in the default config will
still be placed at 0x082000, not truly half-chip-size + 0x2000.
Ideally you should also adjust the len to help detect over sized sections at
link time, but more important is the overall size of the rom which you need to
ensure fits in the space you have allocated for it in your flash layout plan.

Then simply compile and link as you would normally for OTA updates with the SDK
boot loader, except using the linker scripts you've just prepared rather than
the ones supplied with the SDK. Remember when building roms to create them as
'new' type roms (for use with SDK boot loader v1.2+). Or if using my esptool2
use the -boot2 option. Note: the test loads included with rBoot are built with
-boot0 because they do not contain a .irom0.text section (and so the value of
irom0_0_seg in the linker file is irrelevant to them) but 'normal' user apps
always do.

irom checksum
-------------
The SDK boot loader checksum only covers sections loaded into ram (data and some
code). Most of the SDK and user code remains on the flash and that is not
included in the checksum. This means you could attempt to boot a corrupt rom
and, because it looks ok to the boot loader, there will be no attempt to switch
to a backup rom. rBoot improves on this by allowing the .irom0.text section to
be included in the checksum. To enable this uncomment #define BOOT_IROM_CHKSUM
in rboot.h and build your roms with esptool2 using the -iromchksum option.

Big flash support
-----------------
This only needs to be enabled if you wish to be able to memory map more than the
first 8MBit of the flash. Note you can still only map 8Mbit at a time. Use this
if you want to have multiple 1MB roms, or more smaller roms than will fit in
8Mbits. If you have a large flash but only need, for example, two 512KB roms you
do not need to enable this mode.

Support in rBoot is enabled by uncommenting the #define BOOT_BIG_FLASH in
rboot.h.

Thinking about your linker files is either simpler or more complicated,
depending on your usage of the flash. If you intend to use multiple 1MB roms you
will only need one linker file and you only need to link once for OTA updates.
Although when you perform an OTA update the rom will be written to a different
position on the flash, each 8Mbit of flash is mapped (separately) to 0x40200000.
So when any given rom is run the code will appear at the same place in memory
regardless of where it is on the flash. Your base address for the linker would
be 0x40202010. (Actually all but the first rom could base at 0x40200010 (because
they don't need to leave space for rBoot and config) but then you're just making
it more complicated again!)
If you wanted eight 512KB roms you would need two linker files - one for the
first half of any given 8Mbits of flash and another for the second half. Just
remember you are really laying out within a single 8MBit area, which can then be
replicated multiple times on the flash.

Now the clever bit - rBoot needs to hijack the memory mapping code to select
which 8Mbits gets mapped. There is no API for this, but we can override the SDK
function. First we need to slightly modify the SDK library libmain.a, like so:

  xtensa-lx106-elf-objcopy -W Cache_Read_Enable_New libmain.a libmain2.a

This produces a version of libmain with a 'weakened' Cache_Read_Enable_New
function, which we can then override with our own. Modify your Makefile to link
against the library main2 instead of main.
Next add rboot-bigflash.c (from the appcode directory) & rboot.h to your project
- this adds the replacement Cache_Read_Enable_New to your code.

Getting gcc to apply the override correctly can be slightly tricky (I'm not sure
why, it shouldn't be). One option is to add "-u Cache_Read_Enable_New" to your
LD_FLAGS and change the order of objects on the LD command so your objects/.a
file is before the libraries. Another way that seems easier was to #include
rboot-bigflash.c into the main .c file, rather than compiling it to a separate
object file. I can't make any sense of that, but I suggest you uncomment the
message in the Cache_Read_Enable_New function when you first build with it, to
make sure you are getting your version into the rom.

Now when rBoot starts your rom, the SDK code linked in it that normally performs
the memory mapping will delegate part of that task to rBoot code (linked in your
rom, not in rBoot itself) to choose which part of the flash to map.

Integration into other frameworks
---------------------------------
If you wish to integrate rBoot into a development framework (e.g. Sming) you
can set the define RBOOT_INTEGRATION and at compile time the file
rboot-integration.h will be included into the source. This should allow you to
set some platform specific options without having to modify the source of rBoot
which makes it easier to integrate and maintain.
