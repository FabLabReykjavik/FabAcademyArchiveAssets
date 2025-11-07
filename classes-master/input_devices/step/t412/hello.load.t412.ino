//
//  hello.load.t412.ino
//
// tiny412 loading step-response
//
// Neil Gershenfeld 9/7/24
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define sense_pin PIN_PA1 // sense pin
#define settle 100 // settling time
#define samples 1000 // number of samples to accumulate

void setup() {
   Serial.begin(115200); // start serial
   analogSampleDuration(10); // set ADC sampling rate
   analogReadResolution(10); // set ADC resolution
   }

void loop() {
   int32_t count;
   count = 0;
   noInterrupts(); // disable interrupts while measuring
   for (int i = 0; i < samples; ++i) {
      pinMode(sense_pin,OUTPUT); // set sense pin to output
      digitalWriteFast(sense_pin,HIGH); // charge up
      delayMicroseconds(settle); //settle
      pinMode(sense_pin,INPUT); // set sense pin to input
      count += analogRead(sense_pin); // read discharge
      }
   interrupts(); // enable interrupts after measuring
   Serial.println(count); // send count
   Serial.flush(); // finish communicating before measuring
   }
   
