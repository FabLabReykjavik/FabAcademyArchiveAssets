//
// hello.mag.serial.ino
//
// Hall effect serial hello-world
//    Arduino libraries: 1808 bytes, 44% tiny412 memory
//
// Neil Gershenfeld 11/2/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define nsamples 100

void setup() {
   //
   // start serial
   //
   Serial.begin(9600);
   }

void loop() {
   uint32_t result = 0;
   //
   // accumulate readings
   ///
   for (int count = 0; count < nsamples; ++count)
     result += analogRead(2);
   //
   // send framing
   //
   Serial.write(1);
   Serial.write(2);
   Serial.write(3);
   Serial.write(4);
   //
   // send reading
   // 
   Serial.write(result & 255);
   Serial.write((result >> 8) & 255);
   Serial.write((result >> 16) & 255);
   }
