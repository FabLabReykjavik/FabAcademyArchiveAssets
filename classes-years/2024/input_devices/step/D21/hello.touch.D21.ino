//
// hello.touch.D21.ino
//    SAMD21 XIAO FreeTouch hello-world
//
// Neil Gershenfeld 7/23/24
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include "Adafruit_FreeTouch.h"

Adafruit_FreeTouch t6 = Adafruit_FreeTouch(A6,OVERSAMPLE_64,RESISTOR_100K,FREQ_MODE_NONE);
Adafruit_FreeTouch t7 = Adafruit_FreeTouch(A7,OVERSAMPLE_64,RESISTOR_100K,FREQ_MODE_NONE);
Adafruit_FreeTouch t8 = Adafruit_FreeTouch(A8,OVERSAMPLE_64,RESISTOR_100K,FREQ_MODE_NONE);
Adafruit_FreeTouch t9 = Adafruit_FreeTouch(A9,OVERSAMPLE_64,RESISTOR_100K,FREQ_MODE_NONE);
Adafruit_FreeTouch t10 = Adafruit_FreeTouch(A10,OVERSAMPLE_64,RESISTOR_100K,FREQ_MODE_NONE);

int t6min,t7min,t8min,t9min,t10min;

void setup() {
  Serial.begin(115200);  
  while (!Serial);
  t6.begin();
  t7.begin();
  t8.begin();
  t9.begin();
  t10.begin();
  t6min = t7min = t8min = t9min = t10min = 1e6;
  }

void loop() {
  int result;
  //
  // plotting scale limits
  //
  Serial.print(0);
  Serial.print(",");
  Serial.print(300);
  Serial.print(",");
  //
  // read touch
  //
  result = t6.measure();
  if (result < t6min) t6min = result;
  Serial.print(result-t6min);
  Serial.print(",");
  result = t7.measure();
  if (result < t7min) t7min = result;
  Serial.print(result-t7min);
  Serial.print(",");
  result = t8.measure();
  if (result < t8min) t8min = result;
  Serial.print(result-t8min);
  Serial.print(",");
  result = t9.measure();
  if (result < t9min) t9min = result;
  Serial.print(result-t9min);
  Serial.print(",");
  result = t10.measure();
  if (result < t10min) t10min = result;
  Serial.println(result-t10min);
  //
  // pause
  //
  delay(50);
}