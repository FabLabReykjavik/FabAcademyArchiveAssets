//
// hello.t412.3.blink.c
//
// tiny412 blink hello-world, 3-pin UPDI+VCC header
//
// Neil Gershenfeld 1/10/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <avr/io.h>
#include <util/delay.h>

#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin

#define LED_DIR VPORTA.DIR
#define LED_OUT VPORTA.OUT
#define LED_PIN PIN3_bm

int main(void) {
   //
   // initialize LED pin
   //
   CPU_CCP = CCP_IOREG_gc; // unprotect clock
   CLKCTRL.MCLKCTRLB = 0; // turn off prescalar (20 MHz)
   set(LED_DIR,LED_PIN);
   //
   // main loop
   //
   while (1) {
      set(LED_OUT,LED_PIN);
      _delay_ms(100);
      clear(LED_OUT,LED_PIN);
      _delay_ms(100);
      }
   }
