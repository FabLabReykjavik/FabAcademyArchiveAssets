//
// hello.button.D11C.ino
//
// button USB D11C hello-world
//    direct port/register access
//
// Neil Gershenfeld 11/3/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define pin 4
#define button PORT_PA04

void setup() {
   //
   // start serial
   //
   SerialUSB.begin(0);
   //
   // set up button
   //
   REG_PORT_DIRCLR0 = button;
      // input direction
   PORT->Group[0].PINCFG[pin].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
      // input, pull enable
   REG_PORT_OUTSET0 = PORT_PA04;
      // pull-up direction
   }

void loop() {
   //
   // wait for button down
   //
   while ((PORT->Group[0].IN.reg & button) != 0)
      ;
   SerialUSB.write('d');
   //
   // wait for button up
   //
   while ((PORT->Group[0].IN.reg & button) == 0)
      ;
   SerialUSB.write('u');
   }
