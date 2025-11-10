//
// hello.ESP32-WROOM.SPP.ino
//
// ESP32 Bluetooth Serial Port Protocol hello-world
//
// Neil Gershenfeld 11/13/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("hello.ESP32-WROOM.SPP");
  printf("\nBluetooth ready for pairing and serial communication\n\n");
  }

void loop() {
  if (Serial.available())
    SerialBT.write(Serial.read());
  if (SerialBT.available())
    Serial.write(SerialBT.read());
  }
