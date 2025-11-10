//
// hello.mag.USB.ino
//
// Hall effect USB hello-world
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
   SerialUSB.begin(0);
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
   SerialUSB.write(1);
   SerialUSB.write(2);
   SerialUSB.write(3);
   SerialUSB.write(4);
   //
   // send reading
   // 
   SerialUSB.write(result & 255);
   SerialUSB.write((result >> 8) & 255);
   SerialUSB.write((result >> 16) & 255);
   }
