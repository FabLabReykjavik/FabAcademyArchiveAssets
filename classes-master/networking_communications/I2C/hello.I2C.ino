//
// hello.I2C.ino
//
// I2C hello-world scanner
//
// Neil Gershenfeld 12/8/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <Wire.h>

void setup() {
   Serial.begin(115200);
   Wire.begin();
   }

uint8_t address,count;

void loop() {
   count = 0;
   Serial.println("start scan ...");
   for (address = 1; address < 126; address++) {
      Wire.beginTransmission (address);
      if (Wire.endTransmission () == 0) {
         count += 1;
         Serial.print("   found ");
         Serial.print(address,DEC);
         Serial.print(" (0x");
         Serial.print(address,HEX);
         Serial.println (")");
         }
      }
   if (count == 0)
      Serial.println("   no devices found");
   delay(1000);
   }
