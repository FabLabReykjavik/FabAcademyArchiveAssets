//
// hello.stepper.bipolar.D11C.ino
//
// bipolar stepper D11C hello-world
//
// Neil Gershenfeld 11/12/22
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
#define compare(value,pin) (value > pulse) ? set(pin) : clear(pin)
#define A2pin (PORT_PA15) // H-bridge output pins
#define A1pin (PORT_PA14) // "
#define B2pin (PORT_PA08) // "
#define B1pin (PORT_PA05) // "
#define on 40 // PWM on count; ~0.7 us/tick
#define off 10 // PWM off count
#define pulse_count (on+off) // PWM total count
#define PWM_count 50 // number of PWM cycles
#define step_count 100 // number of steps
//
// single step
//
void step(uint32_t A1value, uint32_t A2value, uint32_t B1value, uint32_t B2value) {
   for (uint32_t count = 0; count < PWM_count; ++count) {
      for (uint32_t pulse = 0; pulse < pulse_count; ++pulse) {
         compare(A1value,A1pin);
         compare(A2value,A2pin);
         compare(B1value,B1pin);
         compare(B2value,B2pin);
         }
      }
   }
//
// clockwise full stepping
//
void step_full_cw() {
   step(on,0,on,0);
   step(0,on,on,0);
   step(0,on,0,on);
   step(on,0,0,on);
   }
//
// clockwise half stepping
//
void step_half_cw() {
   step(on,0,0,0);
   step(on,0,on,0);
   step(0,0,on,0);
   step(0,on,on,0);
   step(0,on,0,0);
   step(0,on,0,on);
   step(0,0,0,on);
   step(on,0,0,on);
   }
//
// counter-clockwise full stepping
//
void step_full_ccw() {
   step(on,0,0,on);
   step(0,on,0,on);
   step(0,on,on,0);
   step(on,0,on,0);
   }
//
// counter-clockwise half stepping
//
void step_half_ccw() {
   step(on,0,0,0);
   step(on,0,0,on);
   step(0,0,0,on);
   step(0,on,0,on);
   step(0,on,0,0);
   step(0,on,on,0);
   step(0,0,on,0);
   step(on,0,on,0);
   }
void setup() {
   //
   // initialize H-bridge pins
   //
   clear(A1pin);
   output(A1pin);
   clear(A2pin);
   output(A2pin);
   clear(B1pin);
   output(B1pin);
   clear(B2pin);
   output(B2pin);
   }
void loop() {
   //
   // main loop
   //
   while (1) {
      //
      // full step
      //
      for (uint32_t i = 0; i < step_count; ++i)
         step_full_cw();
      for (uint32_t i = 0; i < step_count; ++i)
         step_full_ccw();
      //
      // half step
      //
      for (uint32_t i = 0; i < step_count; ++i)
         step_half_cw();
      for (uint32_t i = 0; i < step_count; ++i)
         step_half_ccw();
      }
   }
