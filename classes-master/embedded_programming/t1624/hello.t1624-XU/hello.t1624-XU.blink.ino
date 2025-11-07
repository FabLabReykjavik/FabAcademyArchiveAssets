//
// hello.t1624-XU.blink.ino
//
// ATtiny1624-XU blink
//
// Neil Gershenfeld 6/15/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define LED_pin PIN_PA4

void setup() {
  pinMode(LED_pin,OUTPUT);
  }

void loop() {
  digitalWrite(LED_pin,LOW);
  delay(500);
  digitalWrite(LED_pin,HIGH);
  delay(500); 
  }
