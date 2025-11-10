//
// hello.t412.blink.3.ino
//
// tiny412 blink hello-world, 3-pin UPDI+VCC header
//
// Neil Gershenfeld 1/10/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define LED 4

void setup() {
   pinMode(LED,OUTPUT);
   }

void loop() {
   digitalWrite(LED,HIGH);
   delay(100);
   digitalWrite(LED,LOW);
   delay(100);
   }
