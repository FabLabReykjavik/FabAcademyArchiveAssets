//
// hello.4754.C3.ino
//
// Seeed XIAO ESP32C3 IMU hello-world
//    Adafruit 4754: BNO085
//
// Neil Gershenfeld 10/20/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//
// add XIAO_ESP32C3 board
//    https://espressif.github.io/arduino-esp32/package_esp32_index.json
//
// solder P0 jumper for UART RVC mode
//

//#include <HardwareSerial.h>

//HardwareSerial IMU(0);

void setup() {
  Serial.begin();
  //IMU.begin(115200,SERIAL_8N1,D7,D6);
  }

uint8_t c0,c1,buf[13];
int16_t yaw,pitch,roll,ax,ay,az;

void loop() {
  Serial.println(123);
  /*
  //
  // wait for framing
  //
  c0 = 0;
  while (true) {
    if (IMU.available()) {
      c0 = c1;
      c1 = IMU.read();
      if ((c0 == 170) && (c1 == 170)) {
        //
        // read data
        //
        IMU.readBytes(buf,13);
        break;
        }
      }
    }
  //
  // format data
  //
  yaw = buf[1]+256*buf[2];
  pitch = buf[3]+256*buf[4];
  roll = buf[5]+256*buf[6];
  ax = buf[7]+256*buf[8];
  ay = buf[9]+256*buf[10];
  az = buf[11]+256*buf[12];
  //
  // two's complement
  //
  if (yaw > 32767) yaw = yaw-65536;
  if (pitch > 32767) pitch = pitch-65536;
  if (roll > 32767) roll = roll-65536;
  if (ax > 32767) ax = ax-65536;
  if (ay > 32767) ay = ay-65536;
  if (az > 32767) az = az-65536;
  //
  // degrees
  //
  yaw = yaw/100;
  pitch = pitch/100;
  roll = roll/100;
  //
  // send data
  //
  Serial.print(yaw);
  Serial.print(',');
  Serial.print(pitch);
  Serial.print(',');
  Serial.print(roll);
  Serial.print(',');
  Serial.print(ax);
  Serial.print(',');
  Serial.print(ay);
  Serial.print(',');
  Serial.println(az);
  */
  }