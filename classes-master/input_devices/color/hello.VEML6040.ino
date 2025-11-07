//
// hello.VEML6040.ino
//
// VEML6040 color sensor hello-world
//
// Neil Gershenfeld 12/8/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <Wire.h>

#define address 0x10
#define conf 0x00
#define red 0x08
#define green 0x09
#define blue 0x0A
#define white 0x0B

uint16_t read(uint8_t command) {
   uint16_t data = 0; 
   Wire.beginTransmission(address);
   Wire.write(command);
   Wire.endTransmission(false);
   Wire.requestFrom(address,2);
   data = Wire.read(); 
   data |= Wire.read() << 8;
   return data;
   }

void setup() {
   Serial.begin(115200);
   Wire.begin();
   Wire.beginTransmission(address);  
   Wire.write(conf); // configuration
   Wire.write(2 << 4); // low byte: 160 ms, auto mode, enable
   Wire.write(0); // no high byte
   Wire.endTransmission(); 
   }

void loop() {
   Serial.print("  Red: ");
   Serial.println(read(red));
   Serial.print("Green: ");
   Serial.println(read(green));
   Serial.print(" Blue: ");
   Serial.println(read(blue));
   Serial.print("White: ");
   Serial.println(read(white));
   }
