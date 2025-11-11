//
// hello.broadhop-LED.t1624.ino
//    broad-hop LED tiny1624 hello-world
//
// Neil Gershenfeld 11/14/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define LED1 1
#define LED2 2
#define LED3 3
#define RxOut 4
#define PortIn Serial1
#define PortOut Serial

char cmd;
uint8_t arg,addr,address;

void setup() {
   delay(1000);
   PortIn.begin(921600);
   PortOut.begin(921600);
   pinMode(LED1,OUTPUT);
   pinMode(LED2,OUTPUT);
   pinMode(LED3,OUTPUT);
   }

void loop() {
  //
  // wait for a command
  //
  while (PortIn.available() == 0);
  cmd = PortIn.read();
  //
  // all LED command?
  //
  if (cmd == 'L') {
    while (PortIn.available() == 0);
    arg = PortIn.read();
    if (arg & 0b1)
      digitalWriteFast(LED1,HIGH);
    else
      digitalWriteFast(LED1,LOW);
    if (arg & 0b10)
      digitalWriteFast(LED2,HIGH);
    else
      digitalWriteFast(LED2,LOW);
    if (arg & 0b100)
      digitalWriteFast(LED3,HIGH);
    else
      digitalWriteFast(LED3,LOW);
    }
  //
  // individual LED command?
  //
  else if (cmd == 'l') {
    while (PortIn.available() == 0);
    addr = PortIn.read();
    while (PortIn.available() == 0);
    arg = PortIn.read();
    if (addr == address) {
      if (arg & 0b1)
        digitalWriteFast(LED1,HIGH);
      else
        digitalWriteFast(LED1,LOW);
      if (arg & 0b10)
        digitalWriteFast(LED2,HIGH);
      else
        digitalWriteFast(LED2,LOW);
      if (arg & 0b100)
        digitalWriteFast(LED3,HIGH);
      else
        digitalWriteFast(LED3,LOW);
      }
    }
  //
  // address assignment command?
  //
  else if (cmd == 'a') {
    if (digitalRead(RxOut) == LOW) {
      //
      // nothing connected, last node
      //
      address = 0;
      PortIn.write(0);
      }
    else {
      //
      // something connected, wait for hop count
      //
      while (PortOut.available() == 0);
      arg = PortOut.read();
      address = arg+1;
      PortIn.write(address);
      }
    }
  }
