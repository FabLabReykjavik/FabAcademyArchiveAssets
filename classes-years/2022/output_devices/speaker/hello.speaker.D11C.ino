//
// hello.speaker.D11C.ino
//
// speaker D11C hello-world
//
// Neil Gershenfeld 11/6/22
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define delay(n) {volatile uint32_t i; for (i = 0; i < n; ++i) {}}
#define pwm_cycle_sec 5.7e-6 // for on+off count = 10

void setup() {
   REG_PORT_DIR0 = PORT_PA05; // set pin to output
   }

void loop() {
   //
   // 1 kHz square wave
   //
   uint32_t frequency_Hz = 1000;
   float ontime_sec = 1;
   uint32_t wave_cycle,wave_cycle_count = frequency_Hz*ontime_sec;
   uint32_t on_count = 5,off_count = 5;
   uint32_t pwm_cycle,pwm_cycle_count = 1/(frequency_Hz*pwm_cycle_sec);
   for (wave_cycle = 0; wave_cycle < wave_cycle_count; ++wave_cycle) {
      for (pwm_cycle = 0; pwm_cycle < pwm_cycle_count; ++pwm_cycle) {
         PORT_IOBUS->Group[0].OUTSET.reg = PORT_PA05;
         delay(on_count)
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(off_count)
         }
      for (pwm_cycle = 0; pwm_cycle < pwm_cycle_count; ++pwm_cycle) {
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(on_count)
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(off_count)
         }
      }
   //
   // 500 Hz square wave
   //
   frequency_Hz = 500;
   ontime_sec = 1;
   wave_cycle_count = frequency_Hz*ontime_sec;
   on_count = 5,off_count = 5;
   pwm_cycle_count = 1/(frequency_Hz*pwm_cycle_sec);
   for (wave_cycle = 0; wave_cycle < wave_cycle_count; ++wave_cycle) {
      for (pwm_cycle = 0; pwm_cycle < pwm_cycle_count; ++pwm_cycle) {
         PORT_IOBUS->Group[0].OUTSET.reg = PORT_PA05;
         delay(on_count)
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(off_count)
         }
      for (pwm_cycle = 0; pwm_cycle < pwm_cycle_count; ++pwm_cycle) {
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(on_count)
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(off_count)
         }
      }
   //
   // 1 kHz square wave, reduce volume
   //
   frequency_Hz = 1000;
   ontime_sec = 1;
   wave_cycle_count = frequency_Hz*ontime_sec;
   on_count = 2,off_count = 8;
   pwm_cycle_count = 1/(frequency_Hz*pwm_cycle_sec);
   for (wave_cycle = 0; wave_cycle < wave_cycle_count; ++wave_cycle) {
      for (pwm_cycle = 0; pwm_cycle < pwm_cycle_count; ++pwm_cycle) {
         PORT_IOBUS->Group[0].OUTSET.reg = PORT_PA05;
         delay(on_count)
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(off_count)
         }
      for (pwm_cycle = 0; pwm_cycle < pwm_cycle_count; ++pwm_cycle) {
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(on_count)
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(off_count)
         }
      }
   //
   // 500 Hz square wave, reduce volume
   //
   frequency_Hz = 500;
   ontime_sec = 1;
   wave_cycle_count = frequency_Hz*ontime_sec;
   on_count = 2,off_count = 8;
   pwm_cycle_count = 1/(frequency_Hz*pwm_cycle_sec);
   for (wave_cycle = 0; wave_cycle < wave_cycle_count; ++wave_cycle) {
      for (pwm_cycle = 0; pwm_cycle < pwm_cycle_count; ++pwm_cycle) {
         PORT_IOBUS->Group[0].OUTSET.reg = PORT_PA05;
         delay(on_count)
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(off_count)
         }
      for (pwm_cycle = 0; pwm_cycle < pwm_cycle_count; ++pwm_cycle) {
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(on_count)
         PORT_IOBUS->Group[0].OUTCLR.reg = PORT_PA05;
         delay(off_count)
         }
      }
   }

  
