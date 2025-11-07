//
// hello.TLE493D.t412.softwire.ino
//    TLE493D vector magnetometer tiny412 hello-world
//    SoftWire I2C library version
//
// Neil Gershenfeld 3/29/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <SoftWire.h>

#define address 0x35
#define SDA PIN_PA1
#define SCL PIN_PA2

char Tx[16];
char Rx[16];

SoftWire SWire(SDA,SCL);

void setup() {
   //
   // start serial and I2C
   //
   Serial.begin(115200);
   SWire.setTxBuffer(Tx,sizeof(Tx));
   SWire.setRxBuffer(Rx,sizeof(Rx));
   SWire.setDelay_us(5);
   SWire.setTimeout(1000);
   SWire.begin();
   //
   // reset TLE493D
   //
   SWire.beginTransmission(0);
   SWire.write(0xFF);
   SWire.endTransmission();
   SWire.beginTransmission(0);
   SWire.write(0xFF);
   SWire.endTransmission();
   SWire.beginTransmission(0);
   SWire.write(0x00);
   SWire.endTransmission();
   SWire.beginTransmission(0);
   SWire.write(0x00);
   SWire.endTransmission();
   delayMicroseconds(50);
   //
   // configure TLE493D
   //
   SWire.beginTransmission(address);
   SWire.write(0x10);
   SWire.write(0x28);
      // config register 0x10
      // ADC trigger on read after register 0x05
      // short-range sensitivity
   SWire.write(0x15);
      // mode register 0x11
      // 1-byte read protocol
      // interrupt disabled
      // master controlled mode
   SWire.endTransmission();
   }

void loop() {
   uint8_t v0,v1,v2,v3,v4,v5;
   //
   // read data
   //
   SWire.requestFrom(address,6);
   v0 = SWire.read();
   v1 = SWire.read();
   v2 = SWire.read();
   v3 = SWire.read();
   v4 = SWire.read();
   v5 = SWire.read();
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
