//
// hello.t412.blink.ino
//
// tiny412 blink hello-world
//
// Neil Gershenfeld 12/8/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <avr/io.h>

#define LED_AVR PIN2_bm
#define LED_ARDUINO 3

void setup() {
   PORTA.DIRSET = LED_AVR;
   //pinMode(LED_ARDUINO,OUTPUT); // does the same thing
   }

void loop() {
   //
   // timing tests
   //
   // Arduino digitalWrite: 3.9 us/bit
   //
   digitalWrite(LED_ARDUINO,HIGH);
   digitalWrite(LED_ARDUINO,LOW);
   //
   // port read-modify-write: 304 ns/bit
   //    6 cycles at 20 MHz
   //
   PORTA.OUT |= LED_AVR;
   PORTA.OUT &= ~LED_AVR;
   //
   // port bit write: 96 ns
   //    2 cycles at 20 MHz
   //
   PORTA.OUTSET = LED_AVR;
   PORTA.OUTCLR = LED_AVR;
   //
   // virtual port write: 50 ns
   //    1 cycle at 20 MHz
   //
   VPORTA.OUT |= LED_AVR;
   VPORTA.OUT &= ~LED_AVR;
   //
   // blink
   //
   digitalWrite(LED_ARDUINO,HIGH);
   delay(100);
   digitalWrite(LED_ARDUINO,LOW);
   delay(100);
   }
