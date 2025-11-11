//
// hello.TB67H451.RP2040.ino
//    XIAO RP2040 TB67H451 H-bridge hello-world
//
// Neil Gershenfeld 11/7/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

uint32_t duty_min = int(0.65*65535);
uint32_t duty_max = int(0.75*65535);
uint32_t delay_ms = 500;

void setup() {
  analogWriteFreq(100000);
  analogWriteRange(65535);
  }

void loop() {
  analogWrite(0,0);
  analogWrite(1,duty_min);
  delay(delay_ms);
  //
  analogWrite(0,duty_min);
  analogWrite(1,0);
  delay(delay_ms);
  //
  analogWrite(0,0);
  analogWrite(1,duty_max);
  delay(delay_ms);
  //
  analogWrite(0,duty_max);
  analogWrite(1,0);
  delay(delay_ms);
  }
