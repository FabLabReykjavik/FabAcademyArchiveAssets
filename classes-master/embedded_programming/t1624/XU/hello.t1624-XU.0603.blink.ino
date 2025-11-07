//
// hello.t1624-XU.0603.blink.ino
// 
// tiny1624 TSSOP 0604 blink hello-world
//
// Neil Gershenfeld 4/7/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define LED PIN_PA4

void setup() {
  pinMode(LED,OUTPUT);
  }

void loop() {
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(150);
  }
