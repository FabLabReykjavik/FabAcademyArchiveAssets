//
// ATP.E5.FTDI.char.c
//
// E5 115200 baud FTDI character bridge
//    ATP port 0 <-> serial port 1
//
// Neil Gershenfeld
// 12/19/15
//
// (c) Massachusetts Institute of Technology 2015
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//
// ATP macros
//
#include "../ATP.XMEGA.E.h"
//
// serial macros
//
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 8.5 // bit delay for 115200 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay
//
// serial pins
//    FTDI daughter board on port 1, without cable crossover
//
#define serial_port VPORTD.OUT
#define serial_direction VPORTD.DIR
#define serial_pins VPORTD.IN
#define serial_pin_in PIN6_bm
#define serial_pin_out PIN4_bm
#define serial_pin_rts PIN7_bm
#define serial_pin_cts PIN5_bm
//
// LED pins
//
#define led_pin PIN5_bm
#define led_port VPORTA.OUT
#define led_direction VPORTA.DIR
//
// get_char
//    read character into rxbyte on pins pin
//    non-blocking -- assumes start bit has been found
//
void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte) {
   *rxbyte = 0;
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
//
// put_char
//    send character in txchar on port pin
//
void put_char(volatile unsigned char *port, unsigned char pin, char txchar) {
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
//
// ATP_process_bit_out
//    pop bit after sending
//
#define ATP_process_bit_out(ports,id) {\
   ATP_pop_bit_byte_out(ports,id);\
   }
//
// ATP_process_bit_in
//    push bit
//    move complete byte to serial, add 1 for testing
//
#define ATP_process_bit_in(ports,id) {\
   ATP_push_bit_byte_in(ports,id);\
   if (ports[id].in.byte.count == 8) {\
      put_char(&serial_port,serial_pin_out,ports[id].in.byte.value+1);\
      ATP_pop_byte_in(ports,id);\
      }\
   }
//
// main
//
int main(void) {
   //
   // main
   //
   static char chr;
   //
   // set up 32 MHz clock
   //
   /*
   OSC.CTRL = OSC_RC32MEN_bm; // enable 32MHz clock
   while (!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for clock to be ready
   CCP = CCP_IOREG_gc; // enable protected register change
   CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
   */
   //
   // set up PLL clock
   //
   //OSC.PLLCTRL = OSC_PLLFAC4_bm; // 2 MHz * 16 = 32 MHz
   OSC.PLLCTRL = OSC_PLLFAC4_bm | OSC_PLLFAC3_bm; // 2 MHz * 24 = 48 MHz, 20.8 ns cycle
   OSC.CTRL = OSC_PLLEN_bm; // enable PLL
   while (!(OSC.STATUS & OSC_PLLRDY_bm)); // wait for PLL to be ready
   CCP = CCP_IOREG_gc; // enable protected register change
   CLK.CTRL = CLK_SCLKSEL_PLL_gc; // switch to PLL
   //
   // initialize output pins
   //
   set(serial_port,serial_pin_out);
   set(serial_direction,serial_pin_out);
   set(led_port,led_pin);
   set(led_direction,led_pin);
   //
   // set up ATP ports
   //
   ATP_init(ports,1,40); // name,number,buffer size
   ATP_setup(ports,0,D,0,1,2,3); // number,vport,BI,TI,BO,TO
   //
   // delay for power-on transients
   //
   _delay_ms(10);
   //
   // event loop
   //
   while (1) {
      //
      // check serial
      //
      if ((serial_pins & serial_pin_in) == 0) {
         get_char(&serial_pins,serial_pin_in,&chr);
         ATP_push_byte_out(ports,0,chr);
         }
      //
      // check ATP
      //
      ATP_scan(ports,0);
      }
   }

