//
// hello.ESP32-WROOM.BLE.ino
//
// ESP32 BLE hello-world
//
// Neil Gershenfeld 11/12/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include "SimpleBLE.h"

SimpleBLE ble;

void setup() {
  Serial.begin(115200);
  ble.begin("ESP32-WROOM");
  }

void loop() {
  ;
  }
