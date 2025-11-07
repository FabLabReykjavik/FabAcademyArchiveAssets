//
// hello.LSM6DSV16X.fusion.ino
//    LSM6DSV16X IMU sensor fusion hello-world
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
//
// I2C register write
//
void I2C_write_reg(uint8_t addr,uint8_t reg,uint8_t value) {
   Wire.beginTransmission(addr);
   Wire.write(reg);
   Wire.write(value);
   if (Wire.endTransmission() != 0)
      Serial.println("I2C_write_reg failure");
   }
//
// I2C register read
//
uint8_t I2C_read_reg(uint8_t addr,uint8_t reg) {
   Wire.beginTransmission(addr);
   Wire.write(reg);
   if (Wire.endTransmission() != 0)
      Serial.println("I2C_read_reg failure");
   Wire.requestFrom(0x6B,1);
   return Wire.read();
   }
//
// Converts uint16_t half-precision number into a uint32_t single-precision number
// from: https://github.com/stm32duino/LSM6DSV16X/blob/main/src/LSM6DSV16XSensor.cpp
//
void npy_halfbits_to_floatbits(uint16_t h,uint32_t *f) {
  uint16_t h_exp,h_sig;
  uint32_t f_sgn,f_exp,f_sig;
  h_exp = (h & 0x7c00u);
  f_sgn = ((uint32_t)h & 0x8000u) << 16;
  switch (h_exp) {
    case 0x0000u: /* 0 or subnormal */
      h_sig = (h & 0x03ffu);
      /* Signed zero */
      if (h_sig == 0) {
        *f = f_sgn;
        return;
        }
      /* Subnormal */
      h_sig <<= 1;
      while ((h_sig & 0x0400u) == 0) {
        h_sig <<= 1;
        h_exp++;
        }
      f_exp = ((uint32_t)(127-15-h_exp)) << 23;
      f_sig = ((uint32_t)(h_sig & 0x03ffu)) << 13;
      *f = f_sgn + f_exp + f_sig;
      return;
    case 0x7c00u: /* inf or NaN */
      /* All-ones exponent and a copy of the significand */
      *f = f_sgn+0x7f800000u+(((uint32_t)(h & 0x03ffu)) << 13);
      return;
    default: /* normalized */
      /* Just need to adjust the exponent and shift */
      *f = f_sgn+(((uint32_t)(h & 0x7fffu)+0x1c000u) << 13);
      return;
    }
  }
//
// Converts uint16_t half-precision number into a float single-precision number
// from: https://github.com/stm32duino/LSM6DSV16X/blob/main/src/LSM6DSV16XSensor.cpp
//
void npy_half_to_float(uint16_t h,float *f) {
  union {
    float ret;
    uint32_t retbits;
    } conv;
  npy_halfbits_to_floatbits(h,&conv.retbits);
  *f = conv.ret;
  }
//
// Compute quaternions
// from: https://github.com/stm32duino/LSM6DSV16X/blob/main/src/LSM6DSV16XSensor.cpp
//
void sflp2q(float *qx,float *qy,float *qz,float *qw,uint16_t x,uint16_t y,uint16_t z) {
  float sumsq = 0;
  npy_half_to_float(x,qx);
  npy_half_to_float(y,qy);
  npy_half_to_float(z,qz);
  sumsq = (*qx)*(*qx)+(*qy)*(*qy)+(*qz)*(*qz);
  if (sumsq > 1.0f) {
    float n = sqrtf(sumsq);
    *qx /= n;
    *qy /= n;
    *qz /= n;
    sumsq = 1.0f;
    }
  *qw = sqrtf(1.0f-sumsq);
  }
//
// IMU integer read
//
void IMU_read_int(uint8_t *tag,int16_t *x,int16_t *y,int16_t *z) {
   uint8_t xlo,xhi,ylo,yhi,zlo,zhi;
   *tag = I2C_read_reg(address,0x78) >> 3; // FIFO_DATA_OUT_TAG
   xlo = I2C_read_reg(address,0x79); // FIFO_DATA_OUT_X_L
   xhi = I2C_read_reg(address,0x7A); // FIFO_DATA_OUT_X_H
   *x = (xhi << 8) | xlo;
   ylo = I2C_read_reg(address,0x7B); // FIFO_DATA_OUT_Y_L
   yhi = I2C_read_reg(address,0x7C); // FIFO_DATA_OUT_Y_H
   *y = (yhi << 8) | ylo;
   zlo = I2C_read_reg(address,0x7D); // FIFO_DATA_OUT_Z_L
   zhi = I2C_read_reg(address,0x7E); // FIFO_DATA_OUT_Z_H
   *z = (zhi << 8) | zlo;
   }
//
// IMU float read
//
void IMU_read_quaternion(uint8_t *tag,float *qx,float *qy,float *qz,float *qw) {
   uint8_t xlo,xhi,ylo,yhi,zlo,zhi;
   uint16_t ix,iy,iz;
   *tag = I2C_read_reg(address,0x78) >> 3; // FIFO_DATA_OUT_TAG
   xlo = I2C_read_reg(address,0x79); // FIFO_DATA_OUT_X_L
   xhi = I2C_read_reg(address,0x7A); // FIFO_DATA_OUT_X_H
   ix = (xhi << 8) | xlo;
   ylo = I2C_read_reg(address,0x7B); // FIFO_DATA_OUT_Y_L
   yhi = I2C_read_reg(address,0x7C); // FIFO_DATA_OUT_Y_H
   iy = (yhi << 8) | ylo;
   zlo = I2C_read_reg(address,0x7D); // FIFO_DATA_OUT_Z_L
   zhi = I2C_read_reg(address,0x7E); // FIFO_DATA_OUT_Z_H
   iz = (zhi << 8) | zlo;
   sflp2q(qx,qy,qz,qw,ix,iy,iz);
   }
//
// setup
//
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
   // initialize gyro and accelerometer
   //
   I2C_write_reg(address,0x10,0x06); // CTRL1: accel on, 120 Hz, high performance
   I2C_write_reg(address,0x11,0x06); // CTRL2: gyro on, 120 Hz, high performance
   I2C_write_reg(address,0x12,0x44); // CTRL3: block data update, auto inc addresses
   I2C_write_reg(address,0x13,0x00); // CTRL4: for interrupts
   I2C_write_reg(address,0x14,0x00); // CTRL5: for interrupts
   I2C_write_reg(address,0x15,0x03); // CTRL6: gyro 1000 dps
   I2C_write_reg(address,0x16,0x00); // CTRL7: analog hub
   I2C_write_reg(address,0x17,0x00); // CTRL8: accel 2g
   I2C_write_reg(address,0x18,0x00); // CTRL9: accel filter
   I2C_write_reg(address,0x19,0x00); // CTRL10: self test
   //
   // initialize FIFO
   //
   I2C_write_reg(address,0x01,0x80);  // FUNC_CFG_ACCESS: enable embedded access
   I2C_write_reg(address,0x04,0b00000010); // EMB_FUNC_EN_A: SFLP_GAME_EN
   I2C_write_reg(address,0x44,0b00010010); // EMB_FUNC_FIFO_EN_A: 0b00010000 gravity 0b00000010 game
   I2C_write_reg(address,0x01,0x00);  // FUNC_CFG_ACCESS: exit embedded access
   I2C_write_reg(address,0x05,0); // EMB_FUNC_EN_B: embedded functions
   I2C_write_reg(address,0x07,0); // FIFO_CTRL1: watermark
   I2C_write_reg(address,0x08,0); // FIFO_CTRL2: modes
   I2C_write_reg(address,0x09,0b01100110); // FIFO_CTRL3: 0 accel gyro not batched 0b01100110 120 Hz gyro and accel batch
   I2C_write_reg(address,0x0A,0b00000110); // FIFO_CTRL4: continuous mode
   I2C_write_reg(address,0x5E,0b01011011); // SFLP_ODR: 120 Hz sensor fusion output data rate
   }
//
// main loop
//
void loop() {
   uint8_t tag;
   int16_t x,y,z;
   float qx,qy,qz,qw;
   //
   // read accelerometer
   //
   while (true) {
      IMU_read_int(&tag,&x,&y,&z);
      if (tag == 2) {
         Serial.print("ax ay az: ");
         Serial.print(x);
         Serial.print(",");
         Serial.print(y);
         Serial.print(",");
         Serial.print(z);
         Serial.println("");
         break;
         }
      }
   //
   // read gyro
   //
   while (true) {
      IMU_read_int(&tag,&x,&y,&z);
      if (tag == 1) {
         Serial.print("rx ry rz: ");
         Serial.print(x);
         Serial.print(",");
         Serial.print(y);
         Serial.print(",");
         Serial.print(z);
         Serial.println("");
         break;
         }
      }
   //
   // read gravity
   //
   while (true) {
      IMU_read_int(&tag,&x,&y,&z);
      if (tag == 0x17) {
         Serial.print("gx gy gz: ");
         Serial.print(x);
         Serial.print(",");
         Serial.print(y);
         Serial.print(",");
         Serial.print(z);
         Serial.println("");
         break;
         }
      }
   //
   // read quaternions
   //
   while (true) {
      IMU_read_quaternion(&tag,&qx,&qy,&qz,&qw);
      if (tag == 0x13) {
         Serial.print("qw qx qy qz: ");
         Serial.print(qw);
         Serial.print(",");
         Serial.print(qx);
         Serial.print(",");
         Serial.print(qy);
         Serial.print(",");
         Serial.print(qz);
         Serial.println("");
         break;
         }
      }
   }
