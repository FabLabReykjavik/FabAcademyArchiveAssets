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
    // input available, read command
    //
    cmd = PortIn.read();
    //
    // type query command?
    //
    if (cmd == '?') {
      //
      // read address
      //
      while (PortIn.available() == 0);
      addr = PortIn.read();
      if (addr == address) {
        //
        // address matches, reply
        //
        PortIn.write('L');
        }
      else {
        //
        // address doesn't match, forward command
        //
        PortOut.write(cmd);
        PortOut.write(addr);
        }
      }
    //
    // all LED command?
    //
    else if (cmd == 'L') {
      //
      // read argument, execute, and forward
      //
      while (PortIn.available() == 0);
      arg = PortIn.read();
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
      PortOut.write(cmd);
      PortOut.write(arg);
      }
    //
    // individual LED command?
    //
    else if (cmd == 'l') {
      //
      // read address and argument
      //
      while (PortIn.available() == 0);
      addr = PortIn.read();
      while (PortIn.available() == 0);
      arg = PortIn.read();
      if (addr == address) {
        //
        // address matches, execute
        //
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
      else {
        //
        // address doesn't match, forward
        //
        PortOut.write(cmd);
        PortOut.write(addr);
        PortOut.write(arg);
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
        // no, increment and forward
        //
        PortOut.write(cmd);
        PortOut.write(address+1);
        }
      }
    //
    // nothing to do, forward command and any arguments
    //
    else {
      PortOut.write(cmd);
      while (PortIn.available() != 0) {
        PortOut.write(PortIn.read());  
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
