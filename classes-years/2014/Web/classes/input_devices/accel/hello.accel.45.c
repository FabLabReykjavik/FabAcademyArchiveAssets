//
// hello.accel.45.c
//
// accelerometer hello-world
//    9600 baud FTDI interface
//
// Neil Gershenfeld 11/1/14
// (c) Massachusetts Institute of Technology 2014
//
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
#define bit_delay_time 102 // bit delay for 9600 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay

#define serial_port PORTB
#define serial_direction DDRB
#define serial_pin_out (1 << PB2)
#define in_port PORTB
#define in_direction DDRB
#define in_pins PINB
#define x_pin (1 << PB3)
#define y_pin (1 << PB4)

#define NLOOP 3

void put_char(volatile unsigned char *port, unsigned char pin, char txchar) {
   //
   // send character in txchar on port pin
   //    assumes line driver (inverts bits)
   //
   // start bit
   //
   clear(*port,pin);
   bit_delay();
   //
   // unrolled loop to write data bits
   //
   if bit_test(txchar,0)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,1)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,2)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,3)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,4)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,5)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,6)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,7)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   //
   // stop bit
   //
   set(*port,pin);
   bit_delay();
   //
   // char delay
   //
   bit_delay();
   }

int main(void) {
   //
   // main
   //
   static unsigned char i;
   static uint16_t xoff,xon,yoff,yon;
   //
   // set clock divider to /1 (8 MHz)
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize output pins
   //
   set(serial_port, serial_pin_out);
   output(serial_direction, serial_pin_out);
   //
   // main loop
   //
   while (1) {
      //
      // read x (approximate loop timing)
      //
      xoff = 0;
      xon = 0;
      for (i = 0; i < NLOOP; ++i) {
         //
         // wait for x high 
         //
         while (0 == pin_test(in_pins,x_pin))
            ;
         //
         // wait for x low
         //
         while (0 != pin_test(in_pins,x_pin))
            ;
         //
         // time x low 
         //
         while (0 == pin_test(in_pins,x_pin))
            xoff += 1;
         //
         // time x high
         //
         while (0 != pin_test(in_pins,x_pin))
            xon += 1;
         }
      //
      // read y (approximate loop timing)
      //
      yoff = 0;
      yon = 0;
      for (i = 0; i < NLOOP; ++i) {
         //
         // wait for y high 
         //
         while (0 == pin_test(in_pins,y_pin))
            ;
         //
         // wait for y low
         //
         while (0 != pin_test(in_pins,y_pin))
            ;
         //
         // time y low 
         //
         while (0 == pin_test(in_pins,y_pin))
            yoff += 1;
         //
         // time y high
         //
         while (0 != pin_test(in_pins,y_pin))
            yon += 1;
         }
      //
      // send framing
      //
      put_char(&serial_port, serial_pin_out, 1);
      put_char(&serial_port, serial_pin_out, 2);
      put_char(&serial_port, serial_pin_out, 3);
      put_char(&serial_port, serial_pin_out, 4);
      //
      // send x
      //
      put_char(&serial_port, serial_pin_out, (xoff & 255));
      put_char(&serial_port, serial_pin_out, ((xoff >> 8) & 255));
      put_char(&serial_port, serial_pin_out, (xon & 255));
      put_char(&serial_port, serial_pin_out, ((xon >> 8) & 255));
      //
      // send y
      //
      put_char(&serial_port, serial_pin_out, (yoff & 255));
      put_char(&serial_port, serial_pin_out, ((yoff >> 8) & 255));
      put_char(&serial_port, serial_pin_out, (yon & 255));
      put_char(&serial_port, serial_pin_out, ((yon >> 8) & 255));
      }
   }
