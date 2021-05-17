.cpu cortex-m7
.fpu softvfp
.syntax unified
.thumb

.section .vectortab,"a",%progbits
                             //       Cortex-M7 interrupt handlers
.word   _estack              // 0 Stack top address
.word   _reset               // 1 Reset
.word   pass                 // 2 NMI
.word   halt                 // 3 Hard Fault
.word   halt                 // 4 MM Fault
.word   halt                 // 5 Bus Fault
.word   halt                 // 6 Usage Fault
.word   0                    // 7 RESERVED
.word   0                    // 8 RESERVED
.word   0                    // 9 RESERVED
.word   0                    // 10 RESERVED
.word   SVC_Handler          // 11 SV call
.word   halt                 // 12 Debug reserved
.word   0                    // 13 RESERVED
.word   PendSV_Handler       // 14 PendSV
.word   SysTick_Handler      // 15 SysTick
                             //         98 STM32 handlers
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,ETH_IRQHandler,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt

.section .text
.global _reset
_reset:
  ldr sp, =_estack
  bl main
  b .

halt:   b .
pass:   bx lr
