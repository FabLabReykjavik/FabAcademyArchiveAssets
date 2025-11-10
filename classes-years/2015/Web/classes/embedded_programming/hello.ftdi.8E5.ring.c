//
// hello.ftdi.8E5.ring.c
//
// 8E5 hello-world ring oscillator
//    solder bridge pins PC0 and PC1
//       http://academy.cba.mit.edu/classes/embedded_programming/hello.ftdi.8E5.png
//    ATXMEGA8E5-AUR
//       http://www.digikey.com/product-detail/en/ATXMEGA8E5-AUR/ATXMEGA8E5-AURCT-ND/4119404
//       $1.17 x 1000
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

#define led_pin PIN4_bm 
#define led_vport_out VPORTA.OUT
#define led_vport_dir VPORTA.DIR

#define ring_pin_out PIN0_bm
#define ring_pin_in PIN1_bm
#define ring_vport_out VPORTC.OUT
#define ring_vport_in VPORTC.IN
#define ring_vport_dir VPORTC.DIR
#define ring_port_out PORTC.OUT
#define ring_port_in PORTC.IN
#define ring_port_dir PORTC.DIR

int main(void) {
   //
   // set up 32 MHz clock
   //
   /*
   OSC.CTRL = OSC_RC32MEN_bm; // enable 32MHz clock
   while (!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for clock to be ready
   CCP = CCP_IOREG_gc; // enable protected register change
   CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
   */
   //
   // set up PLL clock
   //
   //OSC.PLLCTRL = OSC_PLLFAC4_bm; // 2 MHz * 16 = 32 MHz
   OSC.PLLCTRL = OSC_PLLFAC4_bm | OSC_PLLFAC3_bm; // 2 MHz * 24 = 48 MHz
   OSC.CTRL = OSC_PLLEN_bm; // enable PLL
   while (!(OSC.STATUS & OSC_PLLRDY_bm)); // wait for PLL to be ready
   CCP = CCP_IOREG_gc; // enable protected register change
   CLK.CTRL = CLK_SCLKSEL_PLL_gc; // switch to PLL
   //
   // set up port
   //
   led_vport_dir = led_pin;
   led_vport_out = led_pin;
   ring_vport_dir = ring_pin_out;
   //
   // main loop
   //
   while (1) {
      /*
      // toggle on/off with virtual port
      // 8.0 MHz loop at 32 MHz clock, 4 * 31 ns cycle
      // 12.0 MHz loop at 48 MHz clock, 4 * 21 ns cycle
      // 1 cycle on, 1 cycle off, 2 cycle loop
      //
      ring_vport_out = ring_pin_out;
      ring_vport_out = 0;
      */
      /*
      // toggle on/off without virtual port
      // 5.3 MHz loop at 32 MHz clock, 6 * 31 ns cycle
      // 8.0 MHz loop at 48 MHz clock, 6 * 21 ns cycle
      // 2 cycle on, 2 cycle off, 2 cycle loop
      //
      ring_port_out = ring_pin_out;
      ring_port_out = 0;
      */
      /*
      */
      // ring oscillator with bitwise xor
      // 5.3 MHz loop at 32 MHz clock, 6 * 31 ns cycle
      // 8.0 MHz loop at 48 MHz clock, 6 * 21 ns cycle
      // 1 cycle in, 1 cycle xor, 1 cycle shift, 1 cycle out, 2 cycle loop
      //
      ring_vport_out = (((ring_vport_in ^ ring_pin_in)) >> 1);
      /*
      // ring oscillator with bitwise not
      // 3.5 MHz loop at 32 MHz clock, 9 * 31 ns cycle
      // 5.3 MHz loop at 48 MHz clock, 9 * 21 ns cycle
      //
      ring_vport_out = ((~(ring_vport_in)) >> 1);
      */
     }
   }

