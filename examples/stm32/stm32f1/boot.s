.cpu cortex-m0
.thumb

.word   _estack     /* stack top address */
.word   _reset      /* 1 Reset */
.word   spin        /* 2 NMI */
.word   spin        /* 3 Hard Fault */
.word   spin        /* 4 MM Fault */
.word   spin        /* 5 Bus Fault */
.word   spin        /* 6 Usage Fault */
.word   spin        /* 7 RESERVED */
.word   spin        /* 8 RESERVED */
.word   spin        /* 9 RESERVED*/
.word   spin        /* 10 RESERVED */
.word   spin        /* 11 SV call */
.word   spin        /* 12 Debug reserved */
.word   spin        /* 13 RESERVED */
.word   spin        /* 14 PendSV */
.word   spin        /* 15 SysTick */
.word   spin        /* 16 IRQ0 */
.word   spin        /* 17 IRQ1 */
.word   spin        /* 18 IRQ2 */
.word   spin        /* 19 ...   */
		    /* On to IRQ67 */

spin:   b spin

.thumb_func
.global _reset
_reset:
    bl main
    b .
