//
// hello.MinIMU9v5.ino
//
// Pololu MinIMU-9 v5 hello-world
//
// Neil Gershenfeld 11/2/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <Wire.h>
#include <LSM6.h>
#include <LIS3MDL.h>

LSM6 lsm6; // accelerometer, gyro
LIS3MDL lis3; // magnetometer

void init_IMU() {
   lsm6.init();
   lsm6.enableDefault();
   lsm6.writeReg(LSM6::CTRL1_XL,0x3C);
   lsm6.writeReg(LSM6::CTRL2_G,0x4C);
   lis3.init();
   lis3.enableDefault();
   }

void read_IMU() {
   Serial.println("start");
   lsm6.readAcc();
   Serial.println(lsm6.a.x);
   Serial.println(lsm6.a.y);
   Serial.println(lsm6.a.z);
   lsm6.readGyro();
   Serial.println(lsm6.g.x);
   Serial.println(lsm6.g.y);
   Serial.println(lsm6.g.z);
   lis3.read();
   Serial.println(lis3.m.x);
   Serial.println(lis3.m.y);
   Serial.println(lis3.m.z);
   }

void setup() {
   Serial.begin(115200);
   Wire.begin();
   init_IMU();
   }

void loop() {
   read_IMU();
   }
