//
// hello.TB67H451.RP2040.audio.ino
//    XIAO RP2040 TB67H451 H-bridge audio hello-world
//
// Neil Gershenfeld 11/7/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

uint32_t duty_min = int(0.1*65535);
uint32_t duty_max = int(0.4*65535);
uint32_t duration_us = 10000;

void setup() {
  analogWriteFreq(50000);
  analogWriteRange(65535);
  }

void loop() {
  //
  // square wave soft ramp
  //
  for (uint32_t delay_us = 5000; delay_us > 500; delay_us -= 50) {
    uint32_t cycles = duration_us/delay_us;
    for (uint32_t i = 0; i < cycles; ++i) {
      analogWrite(0,0);
      analogWrite(1,duty_min);
      delayMicroseconds(delay_us);
      //
      analogWrite(0,duty_min);
      analogWrite(1,0);
      delayMicroseconds(delay_us);
      }
    }
  //
  // square wave loud ramp
  //
  for (uint32_t delay_us = 5000; delay_us > 500; delay_us -= 50) {
    uint32_t cycles = duration_us/delay_us;
    for (uint32_t i = 0; i < cycles; ++i) {
      analogWrite(0,0);
      analogWrite(1,duty_max);
      delayMicroseconds(delay_us);
      //
      analogWrite(0,duty_max);
      analogWrite(1,0);
      delayMicroseconds(delay_us);
      }
    }
  }
