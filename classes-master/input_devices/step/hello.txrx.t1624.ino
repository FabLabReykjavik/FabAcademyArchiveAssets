//
// hello.txrx.t1624.ino
//
// ATtiny1624 step response
//
// Neil Gershenfeld 11/14/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define rxpin PIN_PA4 // receive pin
#define txpin PIN_PA5 // transmit pin
#define settle 100 // settle time
#define samples 100 // number of samples to accumulate

void setup() {
   Serial.begin(115200); // start serial
   pinMode(txpin,OUTPUT); // set transmit pin to output
   analogSampleDuration(5); // speed up ADC sampling
   analogReadResolution(12); // increase ADC resolution
   }

void loop() {
   int32_t up,down;
   up = down = 0;
   noInterrupts(); // disable interrupts while measuring
   for (int i = 0; i < samples; ++i) {
      digitalWriteFast(txpin,HIGH); // charge up
      up += analogRead(rxpin); // read
      delayMicroseconds(settle); //settle
      digitalWriteFast(txpin,LOW); // charge down
      down += analogRead(rxpin); // read
      delayMicroseconds(settle); // settle
      }
   interrupts(); // enable interrupts after measuring
   Serial.println(up-down); // send difference
   Serial.flush(); // finish communicating before measuring
   }
