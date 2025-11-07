//
// hello.H-bridge.D11C.ino
//
// H-bridge D11C hello-world
//
// Neil Gershenfeld 11/8/22
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define output(pin) (PORT->Group[0].DIRSET.reg = pin)
#define set(pin) (PORT_IOBUS->Group[0].OUTSET.reg = pin)
#define clear(pin) (PORT_IOBUS->Group[0].OUTCLR.reg = pin)
#define PWM_count 5000 // number of PWM cycles
#define on_delay_fast() delayMicroseconds(40) // PWM on time
#define on_delay_slow() delayMicroseconds(20) // PWM on time
#define off_delay() delayMicroseconds(10) // PWM off time
#define pause() delay(500)

#define IN1 PORT_PA15 // IN1
#define IN2 PORT_PA14 // IN2

void setup() {
   //
   // set pins to output
   //
   output(IN1);
   output(IN2);
   }

void loop() {
   uint32_t count;
   //
   // forward fast
   //
   clear(IN2);
   for (count = 0; count < PWM_count; ++count) {
      set(IN1);
      on_delay_fast();
      clear(IN1);
      off_delay();
      }
   pause();
   //
   // reverse fast
   //
   clear(IN1);
   for (count = 0; count < PWM_count; ++count) {
      set(IN2);
      on_delay_fast();
      clear(IN2);
      off_delay();
      }
   pause();
   //
   // forward slow
   //
   clear(IN2);
   for (count = 0; count < PWM_count; ++count) {
      set(IN1);
      on_delay_slow();
      clear(IN1);
      off_delay();
      }
   pause();
   //
   // reverse slow
   //
   clear(IN1);
   for (count = 0; count < PWM_count; ++count) {
      set(IN2);
      on_delay_slow();
      clear(IN2);
      off_delay();
      }
   pause();
   }
