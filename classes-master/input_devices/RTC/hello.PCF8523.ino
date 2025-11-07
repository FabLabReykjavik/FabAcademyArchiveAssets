//
// hello.PCF8523.ino
//    PCF8523 real-time clock hello-world
//       uses: https://github.com/adafruit/RTClib
//       and: https://github.com/embeddedartistry/arduino-printf
//
// Neil Gershenfeld 1/2/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include "RTClib.h"
#include "LibPrintf.h"

#define set_time 0 // 1 to set RTC to compilation system time

RTC_PCF8523 rtc;

char days[7][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

void setup () {
  //
  // start serial
  //
  Serial.begin(115200);
  while (!Serial);
  //
  // begin RTC
  //
  if (!rtc.begin()) {
    Serial.println("RTC begin failed");
    Serial.flush();
    while (1) delay(1000); // halt
    }
  //
  // set time if required
  //
  if (set_time == 1) {
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),now.minute(),
      now.second())); // can add offset for time to flash
    }
  //
  // start RTC
  //
  rtc.start();
  }

void loop () {
  //
  // read RTC
  //
  DateTime now = rtc.now();
  //
  // print time
  //
  printf("%s %d/%d/%d %02d:%02d:%02d\n",
     days[now.dayOfTheWeek()],now.month(),now.day(),now.year(),
     now.hour(),now.minute(),now.second());
  //
  // pause
  //
  delay(1000);
  }
