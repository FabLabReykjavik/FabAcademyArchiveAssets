//
// hello.servo-registers.t412
//
// tiny412 servo hello-world, registers version
//
// Neil Gershenfeld 11/16/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define delaytime 500 // delay time between steps (us)
#define mincount 625 // 0.5 ms
#define maxcount 3125 // 2.5 ms

int angle = 0;

void setup() {
   PORTA.DIRSET = PIN1_bm; // PA1 to output
   PORTA.DIRSET = PIN2_bm; // PA2 to output
   TCA0.SINGLE.CTRLD = 0; // disable split mode
   TCA0.SINGLE.CTRLB =
      (1 << 6) | // enable compare 2
      (1 << 5) | // enable compare 1
      3; // single-slope PWM
   TCA0.SINGLE.CTRLA = 
      (4 << 1) | // divide by 16
      1; // enable counter
   TCA0.SINGLE.PER = 25000; // 50 Hz period at 20 MHz/16 clock
   TCA0.SINGLE.CMP1 = mincount;
   TCA0.SINGLE.CMP2 = mincount;
   }

void loop() {
   for (angle = mincount; angle <= maxcount; ++angle) {
      TCA0.SINGLE.CMP1 = angle; // step compare 1
      delayMicroseconds(delaytime);
      }
   for (angle = mincount; angle <= maxcount; ++angle) {
      TCA0.SINGLE.CMP2 = angle; // step compare 2
      delayMicroseconds(delaytime);
      }
   for (angle = maxcount; angle >= mincount; --angle) {
      TCA0.SINGLE.CMP1 = angle; // step both
      TCA0.SINGLE.CMP2 = angle;
      delayMicroseconds(delaytime);
      }
   }
