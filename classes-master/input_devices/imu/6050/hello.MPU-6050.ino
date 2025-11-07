//
// hello.MPU-6050.ino
//
// MPU-6050 IMU hello-world
//    based on https://github.com/ElectronicCats/imu6050/blob/master/examples/MPU6050_DMP6/MPU6050_DMP6.ino
//
// Neil Gershenfeld 10/20/24
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 imu;

uint8_t status;
uint16_t size;
uint8_t buffer[64];
Quaternion q;
VectorInt16 accel;
VectorInt16 realaccel;
VectorInt16 gyro;
VectorFloat gravity;
float angles[3];

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("initialize IMU"));
  imu.initialize();
  Serial.println(F("test connection"));
  if (imu.testConnection() == false){
    Serial.println("connection failed");
    while(true);
    }
  else {
    Serial.println("connection successful");
    }
  Serial.println(F("initialize DMP"));
  status = imu.dmpInitialize();
  imu.setXGyroOffset(0);
  imu.setYGyroOffset(0);
  imu.setZGyroOffset(0);
  imu.setXAccelOffset(0);
  imu.setYAccelOffset(0);
  imu.setZAccelOffset(0);
  if (status == 0) {
    imu.CalibrateAccel(6);
    imu.CalibrateGyro(6);
    Serial.println(F("active offsets: "));
    imu.PrintActiveOffsets();
    Serial.println(F("enable DMP"));
    imu.setDMPEnabled(true);
    Serial.println(F("DMP ready"));
    size = imu.dmpGetFIFOPacketSize();
    } 
  else {
    Serial.print(F("DMP initialization failed (code "));
    Serial.print(status);
    Serial.println(F(")"));
    }
  }

void loop() {
  if (imu.dmpGetCurrentFIFOPacket(buffer)) {
      imu.dmpGetQuaternion(&q,buffer);
      imu.dmpGetGravity(&gravity,&q);
      imu.dmpGetYawPitchRoll(angles,&q,&gravity);
      imu.dmpGetAccel(&accel,buffer);
      imu.dmpGetLinearAccel(&realaccel,&accel,&gravity);
      Serial.print(angles[0]*180/M_PI);
      Serial.print(",");
      Serial.print(angles[1]*180/M_PI);
      Serial.print(",");
      Serial.print(angles[2]*180/M_PI);        
      Serial.print(",");
      Serial.print(realaccel.x);
      Serial.print(",");
      Serial.print(realaccel.y);
      Serial.print(",");
      Serial.println(realaccel.z);
  }
}