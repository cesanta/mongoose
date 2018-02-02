Esptool2
richardaburton@gmail.com
http://richard.burtons.org/

Esptool2 is a tool for creating rom images for the ESP8266. It is an alternative
to using the SDK supplied shell script/Makefile/python script combo, which is a
mess. It was inspired by the windows esptool v0.0.2 by mamalala and found on
www.esp8266.com but made somewhat simpler in code and usage. It also adds
support for boot loader v1.2+ rom types, which was the main reason I wrote it.

It was written for my own use and the name was simply to distinguish it for the
other version on my system. The 2 is not intended to imply it is better than the
original. The original has since been updated to v0.0.3 which can write to the
flash, but I currently have no intention to add that to esptool2, it is purely a
rom creating utility. It has become an integral part of my build process now and
has added functionality needed for building the rBoot boot loader. Since I have
released rBoot I needed to release this as well.

Run tool for full usage instructions, or look at the code.
