//
// hello.D11C.bridge.ino
//
// D11C USB serial bridge
//
// Neil Gershenfeld 11/30/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

void setup() {
   SerialUSB.begin(0);
   Serial2.begin(115200);
   }

void loop() {
   if (SerialUSB.available())
      Serial2.print((char) SerialUSB.read());
   if (Serial2.available())
      SerialUSB.print((char) Serial2.read());
   }
