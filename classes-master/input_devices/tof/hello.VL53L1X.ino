//
// hello.VL53L1x.ino
//
// VL53L1X hello-world
//    Pololu library
//
// Neil Gershenfeld 10/31/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <Wire.h>
#include <VL53L1X.h>

VL53L1X device;

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
   // set up VL53L1X
   //
   device.setTimeout(500);
   device.init();
   device.setDistanceMode(VL53L1X::Long);
   device.setMeasurementTimingBudget(140000);
   device.startContinuous(140);
   }

void loop() {
   //
   // read VL53L1X
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
   // send reading
   //
   Serial.write(device.ranging_data.range_status);
   Serial.write(device.ranging_data.range_mm & 255);
   Serial.write((device.ranging_data.range_mm >> 8) & 255);
   }
