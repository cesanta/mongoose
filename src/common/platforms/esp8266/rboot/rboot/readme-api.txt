rBoot - User API for rBoot on the ESP8266
------------------------------------------
by Richard A Burton, richardaburton@gmail.com
http://richard.burtons.org/

This provides a few simple APIs for getting & setting rBoot config and for
writing data from OTA updates. API source files are in the appcode directory.

Actual OTA network code is implementation specific and no longer included in
rBoot itself, see the rBoot sample projects for this code (which you can then
use in your own projects).

  rboot_config rboot_get_config();
    Returns rboot_config (defined in rboot.h) allowing you to modify any values
    in it, including the rom layout.

  bool rboot_set_config(rboot_config *conf);
    Saves the rboot_config structure back to sector 2 of the flash, while
    maintaining the contents of the rest of the sector. You can use the rest of
    this sector for your app settings, as long as you protect this structure
    when you do so.

  uint8 rboot_get_current_rom();
    Get the currently selected boot rom (the currently running rom, as long as
    you haven't changed it since boot).

  bool rboot_set_current_rom(uint8 rom);
    Set the current boot rom, which will be used when next restarted.

  rboot_write_status rboot_write_init(uint32 start_addr);
    Call once before starting to pass data to write to the flash. start_addr is
    the address on the SPI flash to write from. Returns a status structure which
    must be passed back on each write. The contents of the structure should not
    be modified by the calling code.
	
  bool rboot_write_flash(rboot_write_status *status, uint8 *data, uint16 len);
    Call repeatedly to write data to the flash, starting at the address
    specified on the prior call to rboot_write_init. Current write position is
    tracked automatically. This method is likely to be called each time a packet
    of OTA data is received over the network.
