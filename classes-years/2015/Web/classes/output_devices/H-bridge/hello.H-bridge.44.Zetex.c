//
//
// hello.H-bridge.44.Zetex.c
//
// H-bridge hello-world
//
// Neil Gershenfeld
// 11/13/10
//
// (c) Massachusetts Institute of Technology 2010
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
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set

#define gate_delay() _delay_us(5) // delay between switching gates
#define on_delay() _delay_us(1) // PWM on time
#define fast_off_delay() _delay_us(1) // PWM fast off time
#define medium_off_delay() _delay_us(10) // PWM medium off time
#define slow_off_delay() _delay_us(20) // PWM slow off time
#define PWM_count 10000 // number of PWM cycles
#define cycle_count 5 // number of speed cycles

#define MOSFET_port PORTA // MOSFET port
#define MOSFET_direction DDRA // MOSFET direction
#define GRP (1 << PA0) // right PMOSFET gate
#define GRN (1 << PA1) // right NMOSFET gate
#define GLN (1 << PA2) // left NMOSFET gate
#define GLP (1 << PA3) // left PMOSFET gate

int main(void) {
   //
   // main
   //
   static uint16_t count;
   static uint8_t cycle;
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize H-bridge pins
   //
   clear(MOSFET_port, GRP);
   output(MOSFET_direction, GRP);
   clear(MOSFET_port, GLP);
   output(MOSFET_direction, GLP);
   set(MOSFET_port, GRN);
   output(MOSFET_direction, GRN);
   set(MOSFET_port, GLN);
   output(MOSFET_direction, GLN);
   //
   // main loop
   //
   while (1) {
      for (cycle = 0; cycle < cycle_count; ++cycle) {
         //
         // turn right slow
         //
         clear(MOSFET_port, GLN);
         set(MOSFET_port, GRN);
         gate_delay();
         for (count = 0; count < PWM_count; ++count) {
            set(MOSFET_port, GLP);
            on_delay();
            clear(MOSFET_port, GLP);
            slow_off_delay();
            }
         //
         // turn left slow
         //
         clear(MOSFET_port, GRN);
         set(MOSFET_port, GLN);
         gate_delay();
         for (count = 0; count < PWM_count; ++count) {
            set(MOSFET_port, GRP);
            on_delay();
            clear(MOSFET_port, GRP);
            slow_off_delay();
            }
         }
      for (cycle = 0; cycle < cycle_count; ++cycle) {
         //
         // turn right medium 
         //
         clear(MOSFET_port, GLN);
         set(MOSFET_port, GRN);
         gate_delay();
         for (count = 0; count < PWM_count; ++count) {
            set(MOSFET_port, GLP);
            on_delay();
            clear(MOSFET_port, GLP);
            medium_off_delay();
            }
         //
         // turn left medium
         //
         clear(MOSFET_port, GRN);
         set(MOSFET_port, GLN);
         gate_delay();
         for (count = 0; count < PWM_count; ++count) {
            set(MOSFET_port, GRP);
            on_delay();
            clear(MOSFET_port, GRP);
            medium_off_delay();
            }
         }
      for (cycle = 0; cycle < cycle_count; ++cycle) {
         //
         // turn right fast
         //
         clear(MOSFET_port, GLN);
         set(MOSFET_port, GRN);
         gate_delay();
         for (count = 0; count < PWM_count; ++count) {
            set(MOSFET_port, GLP);
            on_delay();
            clear(MOSFET_port, GLP);
            fast_off_delay();
            }
         //
         // turn left fast
         //
         clear(MOSFET_port, GRN);
         set(MOSFET_port, GLN);
         gate_delay();
         for (count = 0; count < PWM_count; ++count) {
            set(MOSFET_port, GRP);
            on_delay();
            clear(MOSFET_port, GRP);
            fast_off_delay();
            }
         }
      }
   }
