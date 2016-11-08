Mongoose Web Server example without an RTOS
===========================================

This project sets up a simple Web server using the Mongoose Web Server and
Networking library.

This project is based on the [LPC4088 QuickStart Board](https://www.embeddedartists.com/products/boards/lpc4088_qsb.php) from Embedded Artists.

Please download the [modified LPCOpen SDK](https://www.embeddedartists.com/sites/default/files/support/qsb/lpc4088/lpcopen_2_10_lpcxpresso_arm_university_4088qsb.zip)
and unpack it into the `LPCOpen_QSB` directory.

This project depends on `lpc_chip_40xx` and `lpc_board_ea_devkit_4088` (for drivers)
and the `webserver` example project (for LWIP). Please import them along with this project.

By default, the project uses DHCP to obtain an IP address.
If you want to configure a static IP address, please edit the settings at the top of `example/src/main.c`.
