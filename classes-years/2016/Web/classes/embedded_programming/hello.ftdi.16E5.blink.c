//
// hello.ftdi.16E5.blink.c
//
// 16E5 blink
//
// Neil Gershenfeld
// 10/27/15
//
// (c) Massachusetts Institute of Technology 2015
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#include <avr/io.h>
#include <util/delay.h>

#define VPORTA VPORT0 // pre-mapped virtual port A
#define VPORTC VPORT1 // pre-mapped virtual port C
#define VPORTD VPORT2 // pre-mapped virtual port D
#define VPORTR VPORT3 // pre-mapped virtual port R

#define DELAY 100 // blink delay
#define LED PIN4_bm // LED bit mask

int main(void) {
   //
   // set up clock
   //
   OSC.CTRL = OSC_RC32MEN_bm; // enable 32MHz clock
   while (!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for clock to be ready
   CCP = CCP_IOREG_gc; // enable protected register change
   CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
   //
   // set up port
   //
   VPORTA.DIR = LED;
   //
   // main loop
   //
   while (1) {
      VPORTA.OUT = LED;
      _delay_ms(DELAY);
      VPORTA.OUT = 0;
     _delay_ms(DELAY);
     }
   }

