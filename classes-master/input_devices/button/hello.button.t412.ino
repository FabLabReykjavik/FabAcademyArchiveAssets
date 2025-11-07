//
// hello.button.t412.ino
//
// button serial tiny412 hello-world
//    direct port/register access: 720 bytes, 17% tiny412 memory
//
// Neil Gershenfeld 11/2/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 102 // bit delay for 9600 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay

#define input_pins PORTA.IN
#define input_pin PIN1_bm
#define input_control PORTA.PIN1CTRL
#define serial_port VPORTA.OUT
#define serial_direction VPORTA.DIR
#define serial_pin PIN6_bm

void put_char(volatile unsigned char *port,unsigned char pin,char txchar) {
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

void setup() {
   //
   // set up serial
   //
   set(serial_port,serial_pin);
   output(serial_direction,serial_pin);
   //
   // turn on button pull-up
   //
   set(input_control,PORT_PULLUPEN_bm);
   }

void loop() {
   //
   // wait for button down
   //
   while (0 != pin_test(input_pins,input_pin))
      ;
   put_char(&serial_port,serial_pin,'d');
   //
   // wait for button up
   //
   while (0 == pin_test(input_pins,input_pin))
      ;
   put_char(&serial_port,serial_pin,'u');
   }
