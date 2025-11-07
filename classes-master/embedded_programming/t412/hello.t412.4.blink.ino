//
// hello.t412.4.blink.ino
//
// tiny412 blink hello-world, 4-pin serial header
//
// Neil Gershenfeld 7/19/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define LED_pin PIN_PA6

void setup() {
  pinMode(LED_pin,OUTPUT);
  }

void loop() {
  digitalWrite(LED_pin,LOW);
  delay(500);
  digitalWrite(LED_pin,HIGH);
  delay(500); 
  }
