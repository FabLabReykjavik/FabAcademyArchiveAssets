//
// hello.TB67H451.RP2040.sample.main.ino
//    XIAO RP2040 TB67H451 H-bridge audio sample hello-world
//    overclock at 250 MHz 
//
// Neil Gershenfeld 11/19/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define sample_length 40876
#define sample_frequency 44100
#define sample_period_us 1e6/sample_frequency
#define sample_scale 0.75
#define pwm_frequency 50000
#define pwm_range 65535

void setup() {
  analogWriteFreq(pwm_frequency);
  analogWriteRange(pwm_range);
  }

uint32_t t0 = 0;
void loop() {
  uint32_t t,count;
  count = 0;
  while (count < sample_length) {
    t = micros();
    if ((t-t0) > sample_period_us) {
      t0 = t;
      if (hello[count] > 0) {
        analogWrite(0,0);
        analogWrite(1,sample_scale*hello[count]);
        }
      else {
        analogWrite(0,-sample_scale*hello[count]);
        analogWrite(1,0);
        }
      count += 1;
      }
    }
  analogWrite(0,0);
  analogWrite(1,0);
  delay(100);
  }
