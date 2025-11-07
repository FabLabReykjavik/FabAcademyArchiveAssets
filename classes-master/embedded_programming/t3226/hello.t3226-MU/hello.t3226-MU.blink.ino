//
// hello.t3226-MU.blink.ino
//
// ATtiny3226-MU blink
//
// Neil Gershenfeld 6/15/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define LED_pin PIN_PC1
#define GND_pin PIN_PB4

void setup() {
  pinMode(LED_pin,OUTPUT);
  pinMode(GND_pin,OUTPUT);
  digitalWrite(GND_pin,LOW);
  }

void loop() {
  digitalWrite(LED_pin,LOW);
  delay(500);
  digitalWrite(LED_pin,HIGH);
  delay(500); 
  }
