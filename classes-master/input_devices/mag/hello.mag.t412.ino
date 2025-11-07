//
// hello.mag.t412.ino
//
// Hall effect tiny412 hello-world
//    direct port/register access: 700 bytes, 17% tiny412 memory
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

#define serial_port VPORTA.OUT
#define serial_direction VPORTA.DIR
#define serial_pin PIN6_bm

#define nsamples 25 // *4 ADC accumulation

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
   // set up ADC
   //
   ADC0.CTRLA = ADC_ENABLE_bm;
      // enable ADC
   ADC0.MUXPOS = ADC_MUXPOS_AIN1_gc;
      // PA1 input
   ADC0.CTRLC = ADC_REFSEL_VDDREF_gc | ADC_PRESC_DIV128_gc;
      // VDD reference, 128x prescaler
   ADC0.CTRLB = ADC_SAMPNUM_ACC4_gc;
      // accumulate 4 samples per conversion
   }

void loop() {
   uint32_t result = 0;
   //
   // accumulate readings
   //
   for (int count = 0; count < nsamples; ++count) {
     ADC0.COMMAND = ADC_STCONV_bm;
     while (ADC0.COMMAND != 0)
        ;
     result += ADC0.RES;
   }
   //
   // send framing
   //
   put_char(&serial_port,serial_pin,1);
   put_char(&serial_port,serial_pin,2);
   put_char(&serial_port,serial_pin,3);
   put_char(&serial_port,serial_pin,4);
   //
   // send reading
   // 
   put_char(&serial_port,serial_pin,result & 255);
   put_char(&serial_port,serial_pin,(result >> 8) & 255);
   put_char(&serial_port,serial_pin,(result >> 16) & 255);
   }
