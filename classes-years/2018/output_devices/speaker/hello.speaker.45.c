//
// hello.speaker.45.c
//
// speaker PWM hello-world
//
// Neil Gershenfeld
// 11/5/18
//
// (c) Massachusetts Institute of Technology 2018
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#include <avr/io.h>
#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin

#define cycle_delay _delay_us(10) // 10 us cycle delay time
#define pmin 25 // minimum half period, in cycle delays
#define pmax 500 // maximum half period
#define pstep 0.95 // half period step factor
#define duration 1000 // period step duration
#define amin 100 // minumum amplitude, in PWM units
#define amax 200 // maximum amplitude
#define astep 50 // amplitude step
#define off 255 // amplitude off

#define MOSFET_pin (1 << PB1)
#define MOSFET_port PORTB
#define MOSFET_direction DDRB

int main(void) {
   //
   // main
   //
   static uint16_t frequency,period,cycle,cycles,delays;
   static uint8_t amplitude;
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   //
   //
   TCCR0A = ((1 << COM0B0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00)); // set OC0B on compare match and set fast PWM mode, 0xFF TOP
   TCCR0B = (1 << CS00); // set timer 0 prescalar to 1
   //
   // initialize MOSFET pin
   //
   clear(MOSFET_port, MOSFET_pin);
   output(MOSFET_direction, MOSFET_pin);
   //
   // main loop
   //
   while (1) {
      //
      // loop over amplitudes
      //
      for (amplitude = amax; amplitude >= amin; amplitude -= astep) {
         //
         // loop over periods
         //
         for (period = pmax; period >= pmin; period *= pstep) {
            //
            // loop over square wave cycles
            //
            cycles = duration/period;
            for (cycle = 0; cycle < cycles; ++cycle) {
               //
               // set PWM current on
               //
               OCR0B = amplitude;
               //
               // loop over delay
               //
               for (delays = 0; delays < period; ++delays)
                  cycle_delay;
               //
               // set PWM current off
               //
               OCR0B = off;
               //
               // loop over delay
               //
               for (delays = 0; delays < period; ++delays)
                  cycle_delay;
               }
            }
         }
      }
   }
