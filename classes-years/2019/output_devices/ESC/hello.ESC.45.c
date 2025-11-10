//
// hello.ESC.45.c
//
// electronic speed controller hello-world, tiny45 version
//    8-bit PWM, 9600 baud FTDI interface
//
// Neil Gershenfeld
// 11/4/19
//
// (c) Massachusetts Institute of Technology 2019
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 102 // bit delay for 9600 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay

#define serial_port PORTB
#define serial_direction DDRB
#define serial_pins PINB
#define serial_pin_out (1 << PB2)
#define serial_pin_in (1 << PB3)
#define PWM_port PORTB
#define PWM_pin (1 << PB4)
#define PWM_direction DDRB

void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte) {
   //
   // read character into rxbyte on pins pin
   //    assumes line driver (inverts bits)
   //
   *rxbyte = 0;
   while (pin_test(*pins,pin))
      //
      // wait for start bit
      //
      ;
   //
   // delay to middle of first data bit
   //
   half_bit_delay();
   bit_delay();
   //
   // unrolled loop to read data bits
   //
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 0);
   else
      *rxbyte |= (0 << 0);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 1);
   else
      *rxbyte |= (0 << 1);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 2);
   else
      *rxbyte |= (0 << 2);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 3);
   else
      *rxbyte |= (0 << 3);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 4);
   else
      *rxbyte |= (0 << 4);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 5);
   else
      *rxbyte |= (0 << 5);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 6);
   else
      *rxbyte |= (0 << 6);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 7);
   else
      *rxbyte |= (0 << 7);
   //
   // wait for stop bit
   //
   bit_delay();
   half_bit_delay();
   }

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

void put_string(volatile unsigned char *port, unsigned char pin, char *str) {
   //
   // print a null-terminated string
   //
   static int index;
   index = 0;
   do {
      put_char(port, pin, str[index]);
      ++index;
      } while (str[index] != 0);
   }

int main(void) {
   //
   // main
   //
   char str[2];
   uint8_t value;
   //
   // set clock divider to /1 (8 MHz)
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize comm pins
   //
   set(serial_port,serial_pin_out);
   output(serial_direction,serial_pin_out);
   input(serial_direction,serial_pin_in);
   //
   // set up timer 1
   //
   TCCR1 = (1 << CS13) | (0 << CS12) | (1 << CS11) | (1 << CS10); // /1024 prescaler
   GTCCR = (1 << PWM1B) | (1 << COM1B1) | (0 << COM1B0); // PWM1B, OC1B cleared on match, ~OC1B not connected
   OCR1C = 153; // 50.2 Hz PWM frequency
   OCR1B = 15; // 2.07 ms PWM pulse
   OCR1B = 7; // 1.03 ms PWM pulse
   OCR1B = 5; // 0.77 ms PWM pulse
   //
   // initialize PWM pin
   //
   clear(PWM_port,PWM_pin);
   output(PWM_direction,PWM_pin);
   //
   // main loop
   //
   while (1) {
      put_string(&serial_port,serial_pin_out,"PWM value (05-15)? ");
      get_char(&serial_pins,serial_pin_in,&str[0]);
      put_char(&serial_port,serial_pin_out,str[0]);
      get_char(&serial_pins,serial_pin_in,&str[1]);
      put_char(&serial_port,serial_pin_out,str[1]);
      put_char(&serial_port,serial_pin_out,10); // new line
      value = atoi(str);
      if ((value < 5) || (value > 15)) {
         put_string(&serial_port,serial_pin_out,"   out of range .7-2 ms");
         put_char(&serial_port,serial_pin_out,10); // new line
         }
      else {
         OCR1B = atoi(str);
         put_string(&serial_port,serial_pin_out,"   updated");
         put_char(&serial_port,serial_pin_out,10); // new line
         }
      }
   }
