1.Examples that support monolithic image download do not need the steps 2-6 to load fw:
    Wi-Fi examples: wifi_cli, wifi_wpa_supplicant, wifi_cli_static, wifi_cert, wifi_cli_fw_dump, wifi_cli_prov, wifi_test_mode
	ncp examples: ncp_device
	coex examples: coex_cli, coex_supplicant_cli

	These examples can load cpu1/2 fw automatically, don't need to load them seperatly.

2. FW image names:
	Production FW, for users:
		rw61x_sb_wifi_a2.bin, for CPU1_wifi of redfinch a2 board
		rw61x_sb_ble_a2.bin, for CPU2_ble of redfinch a2 board
		rw61x_sb_ble_15d4_combo_a2.bin, for CPU2_ble_15.4_combo of redfinch a2 board

3. where to get FW image(Production and mfg_FW):
	In the directory:  /components/conn_fwloader/fw_bin

4. How to load FW(Production and mfg_FW):
	Need to write the FW image to flash first, then the loadservice will download FW when power on.
	For example, the CMD to write CPU1 image to flash in J-link window:
		loadbin C:\xxx\rw61x_sb_wifi_xx.bin,0x08400000

	The CMD to write CPU2_ble image to flash in J-link window:
		loadbin C:\xxx\rw61x_sb_ble_xx.bin,0x08540000

	The CMD to write CPU2_15.4 image to flash in J-link window:
		loadbin C:\xxx\rw61x_sb_ble_15d4_combo_xx.bin,0x085e0000

5. How to generate the C files to be compiled in the monolithic binary:
	In a shell go to directory /components/conn_fwloader. Enter the following command:
	 'python script/fw_bin2c_conv.py -t sb fw_bin'
	or 'python script/fw_bin2c_conv.py -t raw fw_bin':
	This results in generating the C files under fw_bin/A1 and fw_bin/A2 subdirectories.

6. Remap mechanism support
	Whenever the remap feature is active, the flash should not be accessed in direct mode.
	As a consequence, no structure cast should be done on flash direct addresses. Likewise memcpy operations
	should be avoided. Instead all flash accesses must be done via staging buffers in RAM, that 
	get loaded using the mflash driver.