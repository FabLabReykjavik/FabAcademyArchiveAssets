//
// hello.DRV8428.ino
//
// DRV8428 stepper hello-world
//
// Neil Gershenfeld 5/19/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define DIR 4
#define STEP 5

void setup() {
   pinMode(STEP,OUTPUT);
   pinMode(DIR,OUTPUT);
   }

void loop() {
   digitalWrite(DIR,HIGH);
   for (int i = 0; i < 1000; ++i) {
      digitalWrite(STEP,HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP,LOW);
      delayMicroseconds(1000);
      }
   digitalWrite(DIR,LOW);
   for (int i = 0; i < 1000; ++i) {
      digitalWrite(STEP,HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP,LOW);
      delayMicroseconds(1000);
      }
   digitalWrite(DIR,HIGH);
   for (int i = 0; i < 10000; ++i) {
      digitalWrite(STEP,HIGH);
      delayMicroseconds(100);
      digitalWrite(STEP,LOW);
      delayMicroseconds(100);
      }
   digitalWrite(DIR,LOW);
   for (int i = 0; i < 10000; ++i) {
      digitalWrite(STEP,HIGH);
      delayMicroseconds(100);
      digitalWrite(STEP,LOW);
      delayMicroseconds(100);
      }
   }
