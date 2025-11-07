//
// hello.LSM6DSV16X.raw.ino
//
// LSM6DSV16X IMU raw data hello-world
//
// Neil Gershenfeld 5/18/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <Wire.h>

#define address 0x6B

void I2C_write_reg(uint8_t addr,uint8_t reg,uint8_t value) {
   Wire.beginTransmission(addr);
   Wire.write(reg);
   Wire.write(value);
   if (Wire.endTransmission() != 0)
      Serial.println("I2C_write_reg failure");
   }

uint8_t I2C_read_reg(uint8_t addr,uint8_t reg) {
   Wire.beginTransmission(addr);
   Wire.write(reg);
   if (Wire.endTransmission() != 0)
      Serial.println("I2C_read_reg failure");
   Wire.requestFrom(0x6B,1);
   return Wire.read();
   }

void setup() {
   //
   // start serial
   //
   Serial.begin(115200);
   delay(1000);
   //
   // start I2C
   //
   Wire.begin();
   Wire.setClock(1000000);
   //
   // initialize LSM6DSV16X
   //
   Serial.println("initialize LSM6DSV16X");
   I2C_write_reg(address,0x10,0x07); // CTRL1 accel on, 240 Hz, high performance
   I2C_write_reg(address,0x11,0x07); // CTRL2 gyro on, 240 Hz, high performance
   I2C_write_reg(address,0x12,0x44); // CTRL3 block data update, auto inc addresses
   I2C_write_reg(address,0x13,0x00); // CTRL4 for interrupts
   I2C_write_reg(address,0x14,0x00); // CTRL5 for interrupts
   I2C_write_reg(address,0x15,0x04); // CTRL6 gyro 2000 dps
   I2C_write_reg(address,0x16,0x00); // CTRL7 analog hub
   I2C_write_reg(address,0x17,0x01); // CTRL8 accel 4g
   I2C_write_reg(address,0x18,0x00); // CTRL9 accel filter
   I2C_write_reg(address,0x19,0x00); // CTRL10 self test
   }

void loop() {
   uint8_t ret,lo,hi;
   int16_t ax,ay,az,rx,ry,rz;
   //
   // check status register
   //
   ret = I2C_read_reg(address,0x1E); // STATUS_REG
   //
   // gyro data?
   //
   if (ret & 2) { // GDA
      Serial.print("rx ry rz: ");
      lo = I2C_read_reg(address,0x22); //OUTX_L_G
      hi = I2C_read_reg(address,0x23); //OUTX_H_G
      rx = (hi << 8) | lo;
      Serial.print(rx);
      Serial.print(" ");
      lo = I2C_read_reg(address,0x24); //OUTY_L_G
      hi = I2C_read_reg(address,0x25); //OUTY_H_G
      ry = (hi << 8) | lo;
      Serial.print(ry);
      Serial.print(" ");
      lo = I2C_read_reg(address,0x26); //OUTZ_L_G
      hi = I2C_read_reg(address,0x27); //OUTZ_H_G
      rz = (hi << 8) | lo;
      Serial.print(rz);
      Serial.println(" ");
      }
   //
   // accelerometer data?
   //
   if (ret & 1) { // XLDA
      Serial.print("ax ay az: ");
      lo = I2C_read_reg(address,0x28); //OUTX_L_A
      hi = I2C_read_reg(address,0x29); //OUTX_H_A
      ax = (hi << 8) | lo;
      Serial.print(ax);
      Serial.print(" ");
      lo = I2C_read_reg(address,0x2A); //OUTY_L_A
      hi = I2C_read_reg(address,0x2B); //OUTY_H_A
      ay = (hi << 8) | lo;
      Serial.print(ay);
      Serial.print(" ");
      lo = I2C_read_reg(address,0x2C); //OUTZ_L_A
      hi = I2C_read_reg(address,0x2D); //OUTZ_H_A
      az = (hi << 8) | lo;
      Serial.print(az);
      Serial.println("");
      }
   //
   // delay
   //
   delay(100);
   }
