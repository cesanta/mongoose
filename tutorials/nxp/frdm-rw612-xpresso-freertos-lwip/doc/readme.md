# wifi_httpsrv

## Overview
The wifi_httpsrv demo application demonstrates an HTTPServer on the lwIP TCP/IP stack with FreeRTOS.
The user uses an Internet browser to send a request for connection. The board acts as an HTTP server and sends a Web
page back to the PC

Before building the example application select Wi-Fi module macro in the app_config.h. 
(see `#define WIFI_<SoC Name>_BOARD_<Module Name>`).

For more information about Wi-Fi module connection see:
- readme_modules.txt
- [Getting started guide](https://www.nxp.com/document/guide/getting-started-with-nxp-wi-fi-modules-using-i-mx-rt-platform:GS-WIFI-MODULES-IMXRT-PLATFORM)


## Prepare the Demo
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect the Wi-Fi module.
4.  Set symbols WIFI_SSID and WIFI_PASSWORD to match settings of your network prior building of example.
5.  Download the program to the target board.
6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


## Running the demo
1.	When the demo runs, it tries to connect to your network and acquire configuration form DHCP.
2.	On success network configuration will be printed.
3.	On the browser address bar, type IP address of the board.
	The browser should show a web page. The board also advertises itself using mDNS so that it can be accessed using URL http://wifi-http.local.

Please note that your system may not support mDNS out-of-the-box as it is necessary to have an mDNS resolver installed.
For instance Bonjour Print Services for Windows contain such resolver. In case of Linux nss-mdns serves this purpose.
Username admin and password admin is required to access "Authorization example" section of the web page.


## Modifying content of static web pages
To modify content available through the web server you must complete following steps:
1. Modify, add or delete files in folder `boards\<board_name>\lwip_examples\lwip_httpsrv_freertos\webpage`.
2. Run the script file `middleware\lwip\src\apps\httpsrv\mkfs\mkfs.pl <directory name>` to generate new `httpsrv_fs_data.c`. Make sure to execute it from a folder where the file `httpsrv_fs_data.c` is. For example:
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    C:\sdk\boards\<board_name>\lwip_examples\lwip_httpsrv_freertos> C:\sdk\middleware\lwip\src\apps\httpsrv\mkfs\mkfs.pl webpage
    Processing file webpage/auth.html
    Processing file webpage/cgi.html
    Processing file webpage/favicon.ico
    Processing file webpage/help.html
    Processing file webpage/httpsrv.css
    Processing file webpage/index.html
    Processing file webpage/NXP_logo.png
    Processing file webpage/poll.html
    Processing file webpage/request.js
    Processing file webpage/ssi.shtml
    Processing file webpage/welcome.html
    Done.
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

3. Make sure the `httpsrv_fs_data.c` file has been overwritten with the newly generated content.
4. Re-compile the HTTP server application example and download it to your board.

## Supported Boards
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/wifi_examples/common/wifi_examples_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/wifi_examples/common/wifi_examples_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/wifi_examples/common/wifi_examples_readme.md)
