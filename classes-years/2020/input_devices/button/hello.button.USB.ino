//
// hello.button.USB.ino
//
// button USB hello-world
//
// Neil Gershenfeld 11/3/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define button 4

void setup() {
   //
   // start serial
   //
   SerialUSB.begin(0);
   //
   // turn on button pull-up
   //
   pinMode(button,INPUT_PULLUP);
   }

void loop() {
   //
   // wait for button down
   //
   while (digitalRead(button) != LOW)
      ;
   SerialUSB.write('d');
   //
   // wait for button up
   //
   while (digitalRead(button) == LOW)
      ;
   SerialUSB.write('u');
   }
