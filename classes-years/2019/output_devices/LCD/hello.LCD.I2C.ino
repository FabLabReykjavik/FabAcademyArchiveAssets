//
// hello.LCD.I2C.ino
//
// LCD I2C hello-world
//
// Neil Gershenfeld 12/8/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

LiquidCrystal_PCF8574 lcd(0x27); // set LCD address 0x27
int count = 0;

void setup() {
   lcd.begin(16,2); // initialize 16x2 LCD
   //lcd.begin(20,4); // initialize 20x4 LCD
   lcd.setBacklight(255); // turn on backlight
   lcd.home(); // go home
   lcd.clear(); // clear display
   lcd.print("Hello World!"); // print text
   lcd.setCursor(0,1); // move cursor
   lcd.print("Count: ");
   }

void loop() {
   lcd.print(count);
   lcd.setCursor(7,1);
   count += 1;
   }
