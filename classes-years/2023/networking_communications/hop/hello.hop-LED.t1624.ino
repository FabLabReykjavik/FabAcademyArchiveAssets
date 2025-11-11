//
// hello.hop-LED.t1624.ino
//    hop-count LED tiny1624 hello-world
//
// Neil Gershenfeld 12/23/23
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
   digitalWriteFast(LED1,LOW);
   digitalWriteFast(LED2,LOW);
   digitalWriteFast(LED3,LOW);
   pinMode(LED1,INPUT);
   pinMode(LED2,INPUT);
   pinMode(LED3,INPUT);
   }

void loop() {
  //
  // check input port
  //
  if (PortIn.available() != 0) {
    //
    // read and forward command
    //
    cmd = PortIn.read();
    PortOut.write(cmd);
    //
    // all LED command?
    //
    if (cmd == 'L') {
      //
      // read and forward argument
      //
      while (PortIn.available() == 0);
      arg = PortIn.read();
      PortOut.write(arg);
      if (arg & 0b1)
        pinMode(LED1,INPUT);
      else
        pinMode(LED1,OUTPUT);
      if (arg & 0b10)
        pinMode(LED2,INPUT);
      else
        pinMode(LED2,OUTPUT);
      if (arg & 0b100)
        pinMode(LED3,INPUT);
      else
        pinMode(LED3,OUTPUT);
      }
    //
    // individual LED command?
    //
    else if (cmd == 'l') {
      //
      // read and forward address and argument
      //
      while (PortIn.available() == 0);
      addr = PortIn.read();
      PortOut.write(addr);
      while (PortIn.available() == 0);
      arg = PortIn.read();
      PortOut.write(arg);
      if (addr == address) {
        if (arg & 0b1)
          pinMode(LED1,INPUT);
        else
          pinMode(LED1,OUTPUT);
        if (arg & 0b10)
          pinMode(LED2,INPUT);
        else
          pinMode(LED2,OUTPUT);
        if (arg & 0b100)
          pinMode(LED3,INPUT);
        else
          pinMode(LED3,OUTPUT);
        }
      }
    //
    // address assignment command?
    //
    else if (cmd == 'a') {
      //
      // read address
      //
      while (PortIn.available() == 0);
      address = PortIn.read();
      //
      // last node?
      //
      if (digitalRead(RxOut) == LOW) {
        //
        // yes, send back count
        //
        PortIn.write(address);
        }
      else {
        //
        // no, increment and forward address
        //
        PortOut.write(address+1);
        }
      }
    }
  //
  // check output port
  //
  if (PortOut.available() != 0) {
    //
    // send to input port
    //
    PortIn.write(PortOut.read());
    }
  }
