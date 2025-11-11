//
// hello.VL53L0X.D11C.ino
//
// VL53L0X D11C hello-world
//    Pololu library
//
// Neil Gershenfeld 10/31/22
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X device;

#define shutdown_pin (1 << 5)

void setup() {
   SerialUSB.begin(115200);
   delay(1);
   REG_PORT_DIR0 |= shutdown_pin;
   REG_PORT_OUT0 |= shutdown_pin;
   Wire.begin();
   if (!device.init()) {
      SerialUSB.println("init failed");
      }
   }

void loop() {
   SerialUSB.println(device.readRangeSingleMillimeters());
   }
