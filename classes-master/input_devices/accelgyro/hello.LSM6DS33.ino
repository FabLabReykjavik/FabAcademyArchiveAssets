//
// hello.LSD6DS33.ino
//
// LSM6DS33 hello-world
//    Pololu library
//
// Neil Gershenfeld 11/01/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <Wire.h>
#include <LSM6.h>

LSM6 device;

void setup() {
   //
   // set up serial
   //
   Serial.begin(115200);
   //
   // set up I2C
   //
   Wire.begin();
   Wire.setClock(400000);
   //
   // set up LSM6DS33
   //
   device.init();
   device.enableDefault();
   }

void loop() {
   //
   // read LSM6DS33
   //
   device.read();
   //
   // send framing
   //
   Serial.write(1);
   Serial.write(2);
   Serial.write(3);
   Serial.write(4);
   //
   // send readings
   //
   Serial.write(device.a.x & 255);
   Serial.write((device.a.x >> 8) & 255);
   Serial.write(device.a.y & 255);
   Serial.write((device.a.y >> 8) & 255);
   Serial.write(device.a.z & 255);
   Serial.write((device.a.z >> 8) & 255);
   Serial.write(device.g.x & 255);
   Serial.write((device.g.x >> 8) & 255);
   Serial.write(device.g.y & 255);
   Serial.write((device.g.y >> 8) & 255);
   Serial.write(device.g.z & 255);
   Serial.write((device.g.z >> 8) & 255);
   }
