//
// hello.ftdi.45.port.c
//
// tiny45 pin output port hello-world
//
// Neil Gershenfeld 11/17/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <avr/io.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin

#define port PORTB
#define direction DDRB
#define pin (1 << PB1)

int main(void) {
   //
   // main
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize output pin
   //
   output(direction,pin);
   //
   // main loop
   //
   while (1) {
      set(port,pin);
      clear(port,pin);
      set(port,pin);
      clear(port,pin);
      }
   }
