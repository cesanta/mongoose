This is a ESP boot loader stub development environment.

Code produced in this environment can be loaded and executed
in the bootloader environment. Usually it is used to implement
functionality not found in the bootloader.

Stubs can be executed using the `run_stub` command of the modified esptool.py provided.
`wrap_stub` produces a JSON represenattion of the stub that can later be reused
or built into other tools.

Example usage:
  $ make run STUB=stub_flash_size.c PORT=/dev/ttyUSB0
  $ make run STUB=stub_md5.c PORT=/dev/ttyUSB0 PARAMS="0x11000 10000 1"
