//
// hello.ftdi.8E5.echo.c
//
// 8E5 115200 baud FTDI character echo, with flash string
//
// Neil Gershenfeld
// 10/27/15
//
// (c) Massachusetts Institute of Technology 2015
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 8.5 // bit delay for 115200 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay

#define VPORTA VPORT0 // pre-mapped virtual port A
#define VPORTC VPORT1 // pre-mapped virtual port C
#define VPORTD VPORT2 // pre-mapped virtual port D
#define VPORTR VPORT3 // pre-mapped virtual port R

#define serial_port VPORT2.OUT
#define serial_direction VPORT2.DIR
#define serial_pins VPORT2.IN
#define serial_pin_in PIN6_bm
#define serial_pin_out PIN7_bm

#define led_port VPORT0.OUT
#define led_direction VPORT0.DIR
#define led_pin PIN4_bm

#define max_buffer 25

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
   static char chr;
   static char buffer[max_buffer] = {0};
   static int index;
   //
   // set up clock
   //
   OSC.CTRL = OSC_RC32MEN_bm; // enable 32MHz clock
   while (!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for clock to be ready
   CCP = CCP_IOREG_gc; // enable protected register change
   CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
   //
   // initialize output pins
   //
   set(serial_port, serial_pin_out);
   output(serial_direction, serial_pin_out);
   output(led_direction, led_pin);
   //
   // main loop
   //
   index = 0;
   while (1) {
      get_char(&serial_pins, serial_pin_in, &chr);
      set(led_port, led_pin);
      put_string(&serial_port, serial_pin_out, "hello.ftdi.8E5.echo.c: you typed \"");
      buffer[index++] = chr;
      if (index == (max_buffer-1))
         index = 0;
      put_string(&serial_port, serial_pin_out, buffer);
      put_char(&serial_port, serial_pin_out, '\"');
      put_char(&serial_port, serial_pin_out, 10); // new line
      clear(led_port, led_pin);
      }
   }
