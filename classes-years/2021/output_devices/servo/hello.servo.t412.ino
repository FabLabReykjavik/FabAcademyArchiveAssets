//
// hello.servo.t412
//
// tiny412 servo hello-world
//
// Neil Gershenfeld 11/16/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <Servo.h>

#define delaytime 5 // delay time between steps (ms)
#define servopin0 2 // servo 0 pin (PA2)
#define servopin1 3 // servo 1 pin (PA1)

Servo servo0,servo1; //declare servos
int angle = 0;

void setup() {
   servo0.attach(servopin0); // attach servos to pins
   servo1.attach(servopin1);
   }

void loop() {
   for (angle = 0; angle <= 180; ++angle) { // step servo 0
      servo0.write(angle);
      delay(delaytime);
      }
   for (angle = 0; angle <= 180; ++angle) { // step servo 1
      servo1.write(angle);
      delay(delaytime);
      }
   for (angle = 180; angle >= 0; --angle) { // step both
      servo0.write(angle);
      servo1.write(angle);
      delay(delaytime);
      }
   }
