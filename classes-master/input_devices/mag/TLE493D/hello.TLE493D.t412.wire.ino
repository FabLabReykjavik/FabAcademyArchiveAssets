//
// hello.TLE493D.t412.wire.ino
//    TLE493D vector magnetometer tiny412 hello-world
//    Wire I2C library version
//
// Neil Gershenfeld 3/29/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <Wire.h>

#define address 0x35

void setup() {
   //
   // start serial and I2C
   //
   Serial.begin(115200);
   Wire.begin();
   Wire.setClock(400000);
   //
   // reset TLE493D
   //
   Wire.beginTransmission(0);
   Wire.write(0xFF);
   Wire.endTransmission();
   Wire.beginTransmission(0);
   Wire.write(0xFF);
   Wire.endTransmission();
   Wire.beginTransmission(0);
   Wire.write(0x00);
   Wire.endTransmission();
   Wire.beginTransmission(0);
   Wire.write(0x00);
   Wire.endTransmission();
   delayMicroseconds(50);
   //
   // configure TLE493D
   //
   Wire.beginTransmission(address);
   Wire.write(0x10);
   Wire.write(0x28);
      // config register 0x10
      // ADC trigger on read after register 0x05
      // short-range sensitivity
   Wire.write(0x15);
      // mode register 0x11
      // 1-byte read protocol
      // interrupt disabled
      // master controlled mode
   Wire.endTransmission();
   }

void loop() {
   uint8_t v0,v1,v2,v3,v4,v5;
   //
   // read data
   //
   Wire.requestFrom(address,6);
   v0 = Wire.read();
   v1 = Wire.read();
   v2 = Wire.read();
   v3 = Wire.read();
   v4 = Wire.read();
   v5 = Wire.read();
   //
   // send framing
   //
   Serial.write(1);
   Serial.write(2);
   Serial.write(3);
   Serial.write(4);
   //
   // send data
   //
   Serial.write(v0);
   Serial.write(v1);
   Serial.write(v2);
   Serial.write(v3);
   Serial.write(v4);
   Serial.write(v5);
   }
