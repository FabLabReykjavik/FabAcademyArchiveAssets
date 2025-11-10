//
// hello.ftdi.45.port.write.ino
//
// tiny45 pin output digitalWrite hello-world
//
// Neil Gershenfeld 11/17/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

void setup() {
  //
  // set clock divider to /1
  //
  CLKPR = (1 << CLKPCE);
  CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
  //
  // initialize output pin
  //
  pinMode(1,OUTPUT);
  }

void loop() {
  digitalWrite(1,HIGH);
  digitalWrite(1,LOW);
  digitalWrite(1,HIGH);
  digitalWrite(1,LOW);
  }
