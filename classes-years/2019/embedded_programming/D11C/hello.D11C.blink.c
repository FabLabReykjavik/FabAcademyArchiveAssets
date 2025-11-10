//
// hello.D11C.blink.c
//
// SAMD11C LED blink hello-world
//
// Neil Gershenfeld 11/29/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#include <samd11.h>
#define LED (1 << 5) // define LED pin

void delay(uint32_t n) {
   uint32_t i = 0;
   while (i < n) ++i;
   }

int main(void) {
   SYSCTRL->OSC8M.bit.PRESC = 0; // set OSC8M clock prescaler to 1
   REG_PORT_DIR0 |= LED; // set LED pin to output
   while (1) {
      REG_PORT_OUT0 |= LED; // turn on LED
      delay(1000000); // delay
      REG_PORT_OUT0 &= ~LED; // turn off LED
      delay(1000000); // delay
      }
   }
