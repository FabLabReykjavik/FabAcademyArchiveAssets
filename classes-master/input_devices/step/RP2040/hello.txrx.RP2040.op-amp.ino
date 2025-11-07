//
// hello.txtx.RP2040.op-amp.ino
//    RP2040 XIAO transmit-receive step-response op-amp hello-world
//    overclock at 250 MHz
//
// Neil Gershenfeld 7/28/24
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define digitalWriteFast(pin,val) (val ? sio_hw->gpio_set = (1 << pin) : sio_hw->gpio_clr = (1 << pin))
#define digitalReadFast(pin) ((1 << pin) & sio_hw->gpio_in)

#define Rx 26 // receive pin (A0)
#define Tx 1 // transmit pin (D7)
#define settle 20 // settle time
#define samples 5000 // number of samples to accumulate

void setup() {
   Serial.begin(115200);
   }

void loop() {
   }

void setup1() {
   pinMode(Tx,OUTPUT);
   analogReadResolution(12);
   }

void loop1() {
   int32_t up,down;
   up = down = 0;
   for (int i = 0; i < samples; ++i) {
      digitalWriteFast(Tx,HIGH); // charge up
      up += analogRead(Rx); // read
      delayMicroseconds(settle); //settle
      digitalWriteFast(Tx,LOW); // charge down
      down += analogRead(Rx); // read
      delayMicroseconds(settle); // settle
      }
   Serial.println((down-up)/samples); // send difference (op-amp inverts)
   }
