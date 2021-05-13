.cpu cortex-m7
.thumb

.word   _estack              // 0 Stack top address
.word   _reset               // 1 Reset
.word   pass                 // 2 NMI
.word   halt                 // 3 Hard Fault
.word   halt                 // 4 MM Fault
.word   halt                 // 5 Bus Fault
.word   halt                 // 6 Usage Fault
.word   halt                 // 7 RESERVED
.word   halt                 // 8 RESERVED
.word   halt                 // 9 RESERVED
.word   halt                 // 10 RESERVED
.word   SVC_handler          // 11 SV call
.word   halt                 // 12 Debug reserved
.word   halt                 // 13 RESERVED
.word   pending_SV_handler   // 14 PendSV
.word   SysTick_handler      // 15 SysTick
.word   pass                 // 16 IRQ0
.word   halt                 // 17 IRQ1
                             // On to IRQ67
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt
.word   halt,halt,halt,halt,halt,halt,halt,halt,halt,halt

halt:   b halt
pass:   BX lr

.thumb_func
.global _reset
_reset:
    bl main
    b .
