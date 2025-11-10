//
// hello.servo.44.2.c
//
// two-channel software PWM servo motor hello-world
//
// set lfuse to 0x7E for 20 MHz xtal
//
// Neil Gershenfeld
// 4/8/12
//
// (c) Massachusetts Institute of Technology 2012
// Permission granted for experimental and personal use;
// license for commercial sale available from MIT.
//

#include <avr/io.h>
#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define position_delay() _delay_ms(1000)

#define PWM_port PORTA
#define PWM_direction DDRA
#define PWM_pin_0 (1 << PA6)
#define PWM_pin_1 (1 << PA7)

#define loop_count 30

int main(void) {
   //
   // main
   //
   uint8_t i;
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // set PWM pins to output
   //
   clear(PWM_port, PWM_pin_0);
   output(PWM_direction, PWM_pin_0);
   clear(PWM_port, PWM_pin_1);
   output(PWM_direction, PWM_pin_1);
   //
   // main loop
   //
   while (1) {
      //
      // 1 ms on time, both
      //
      for (i = 0; i < loop_count; ++i) {
         set(PWM_port,PWM_pin_0);
         set(PWM_port,PWM_pin_1);
         _delay_us(1000);
         clear(PWM_port,PWM_pin_0);
         clear(PWM_port,PWM_pin_1);
         _delay_us(19000);
         }
      //
      // 1.5 ms on time, both
      //
      for (i = 0; i < loop_count; ++i) {
         set(PWM_port,PWM_pin_0);
         set(PWM_port,PWM_pin_1);
         _delay_us(1500);
         clear(PWM_port,PWM_pin_0);
         clear(PWM_port,PWM_pin_1);
         _delay_us(18500);
         }
      //
      // 2 ms on time, both
      //
      for (i = 0; i < loop_count; ++i) {
         set(PWM_port,PWM_pin_0);
         set(PWM_port,PWM_pin_1);
         _delay_us(2000);
         clear(PWM_port,PWM_pin_0);
         clear(PWM_port,PWM_pin_1);
         _delay_us(18000);
         }
      //
      // 1 ms on time, channel 0
      //
      for (i = 0; i < loop_count; ++i) {
         set(PWM_port,PWM_pin_0);
         _delay_us(1000);
         clear(PWM_port,PWM_pin_0);
         _delay_us(19000);
         }
      //
      // 1 ms on time, channel 1
      //
      for (i = 0; i < loop_count; ++i) {
         set(PWM_port,PWM_pin_1);
         _delay_us(1000);
         clear(PWM_port,PWM_pin_1);
         _delay_us(19000);
         }
      //
      // 1.5 ms on time, channel 0
      //
      for (i = 0; i < loop_count; ++i) {
         set(PWM_port,PWM_pin_0);
         _delay_us(1500);
         clear(PWM_port,PWM_pin_0);
         _delay_us(18500);
         }
      //
      // 1.5 ms on time, channel 1
      //
      for (i = 0; i < loop_count; ++i) {
         set(PWM_port,PWM_pin_1);
         _delay_us(1500);
         clear(PWM_port,PWM_pin_1);
         _delay_us(18500);
         }
      //
      // 2 ms on time, channel 0
      //
      for (i = 0; i < loop_count; ++i) {
         set(PWM_port,PWM_pin_0);
         _delay_us(2000);
         clear(PWM_port,PWM_pin_0);
         _delay_us(18000);
         }
      //
      // 2 ms on time, channel 1
      //
      for (i = 0; i < loop_count; ++i) {
         set(PWM_port,PWM_pin_1);
         _delay_us(2000);
         clear(PWM_port,PWM_pin_1);
         _delay_us(18000);
         }
      }
   }
