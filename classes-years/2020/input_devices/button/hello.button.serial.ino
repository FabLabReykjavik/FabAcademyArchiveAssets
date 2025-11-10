//
// hello.button.serial.ino
//
// button serial hello-world
//    Arduino libraries: 1718 bytes, 41% tiny412 memory
//
// Neil Gershenfeld 11/3/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define button 2

void setup() {
   //
   // start serial
   //
   Serial.begin(9600);
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
   Serial.write('d');
   //
   // wait for button up
   //
   while (digitalRead(button) == LOW)
      ;
   Serial.write('u');
   }
