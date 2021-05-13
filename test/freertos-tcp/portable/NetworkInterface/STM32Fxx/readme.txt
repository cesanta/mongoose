This is a FreeeRTOS+TCP driver that works for both STM32F4xx and STM32F7xx parts.

The code of stm32fxx_hal_eth.c is based on both drivers as provided by ST.

These modules should be included:

    NetworkInterface.c
	stm32fxx_hal_eth.c

It is assumed that one of these words are defined:

	STM32F7xx
	STM32F407xx
	STM32F417xx
	STM32F427xx
	STM32F437xx
	STM32F429xx
	STM32F439xx

The driver has been tested on both Eval and Discovery boards with both STM32F4 and STM32F7.

Recommended settings for STM32Fxx Network Interface:

// Defined in FreeRTOSIPConfig.h

#define ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES   1
#define ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM        1
#define ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM        1
#define ipconfigZERO_COPY_RX_DRIVER                   1
#define ipconfigZERO_COPY_TX_DRIVER                   1
#define ipconfigUSE_LINKED_RX_MESSAGES                1

// Defined in stm32f4xx_hal_conf.h
#define ETH_RXBUFNB                                   3 or 4
#define ETH_TXBUFNB                                   2 or 3
#define ETH_RX_BUF_SIZE                               ( ipconfigNETWORK_MTU + 36 )
#define ETH_TX_BUF_SIZE                               ( ipconfigNETWORK_MTU + 36 )

The best size for 'ETH_RXBUFNB' and 'ETH_TXBUFNB' depends on the speed of the CPU. These macro's define the number of DMA buffers for reception and for transmission.
In general, if the CPU is very fast, you will need less buffers. You can obtain an estimate empirically.

The optimal value of 'ETH_RX_BUF_SIZE' and 'ETH_TX_BUF_SIZE' depends on the actual value of 'ipconfigNETWORK_MTU'.
When MTU is 1500, MTU+36 becomes a well-aligned buffer of 1536 bytes ( 0x600 ).
When MTU is 1200, MTU+48 will make 1248 ( 0x4E0 ), which is also well aligned.

Having well aligned buffers is important for CPU with memory cache. Often the caching system divides memory in blocks of 32 bytes. When two buffers share the same cache buffer, you are bound to see data errors.

Without memory caching, let the size be at least a multiple of 8 ( for DMA ), and make it at least "ipconfigNETWORK_MTU + 14".

STM32F7xx only:

Networkinterface.c will place the 2 DMA tables in a special section called 'first_data'.
In case 'BufferAllocation_1.c' is used, the network packets will also be declared in this section 'first_data'.
As long as the part has no caching, this section can be placed anywhere in RAM.
On an STM32F7 with an L1 data cache, it shall be placed in the first 64KB of RAM, which is always uncached.
The linker script must be changed for this, for instance as follows:

   .data :
   {
     . = ALIGN(4);
     _sdata = .;        // create a global symbol at data start
+    *(.first_data)     // .first_data sections
     *(.data)           // .data sections
     *(.data*)          // .data* sections

     . = ALIGN(4);
     _edata = .;        // define a global symbol at data end
   } >RAM AT> FLASH


The driver contains these files:

	stm32fxx_hal_eth.c
	stm32f2xx_hal_eth.h
	stm32f4xx_hal_eth.h
	stm32f7xx_hal_eth.h
	stm32fxx_hal_eth.h

These files are copied from ST's HAL library. These work both for STM32F4 and STM32F7.
Please remove or rename these files from the HAL distribution that you are using.

