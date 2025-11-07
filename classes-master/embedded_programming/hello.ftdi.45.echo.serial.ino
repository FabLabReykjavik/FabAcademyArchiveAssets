//
// hello.ftdi.45.echo.serial.ino
//
// tiny45 echo hello-world
//    9600 baud FTDI interface, SoftwareSerial
//
// Neil Gershenfeld 11/17/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <SoftwareSerial.h>

#define serial_pin_in 2
#define serial_pin_out 1
#define max_buffer 25

SoftwareSerial Serial(serial_pin_in,serial_pin_out);

void setup() {
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize pins
   //
   pinMode(serial_pin_out,OUTPUT);
   //
   // initialize serial
   //
   Serial.begin(9600);
   }

static int index = 0;
static char chr;
static char buffer[max_buffer] = {0};

void loop() {
   //
   // main loop
   //
   if (Serial.available() > 0) {
      chr = Serial.read();
      Serial.print("hello.ftdi.45.echo.serial: you typed \"");
      buffer[index++] = chr;
      if (index == (max_buffer-1))
         index = 0;
      Serial.print(buffer);
      Serial.println("\"");
      }
   }
