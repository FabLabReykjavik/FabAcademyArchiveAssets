//
// ATP.E5.byte.c
//    ATP E5 byte echo ring oscillator
//
// Neil Gershenfeld
// 12/10/15
//
// (c) Massachusetts Institute of Technology 2015
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//
#include "../ATP.XMEGA.E.h"
//
// I/O pin definitions
//
#define button (VPORTA.IN & PIN6_bm)
#define button_ctrl PORTA.PIN6CTRL
#define led_pin PIN5_bm
#define led_port VPORTA
//
// ATP_process_bit_out
//    pop bit after sending
//
#define ATP_process_bit_out(ports,id) {\
   ATP_pop_bit_byte_out(ports,id);\
   }
//
// ATP_process_bit_in
//    push bit
//    move complete byte from in to out
//
#define ATP_process_bit_in(ports,id) {\
   ATP_push_bit_byte_in(ports,id);\
   if (ports[id].in.byte.count == 8) {\
      ATP_push_byte_out(ports,id,ports[id].in.byte.value);\
      ATP_pop_byte_in(ports,id);\
      }\
   }
//
// main
//
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
   /*
   OSC.PLLCTRL = OSC_PLLFAC4_bm; // 2 MHz * 16 = 32 MHz
   */
   OSC.PLLCTRL = OSC_PLLFAC4_bm | OSC_PLLFAC3_bm; // 2 MHz * 24 = 48 MHz, 20.8 ns cycle
   OSC.CTRL = OSC_PLLEN_bm; // enable PLL
   while (!(OSC.STATUS & OSC_PLLRDY_bm)); // wait for PLL to be ready
   CCP = CCP_IOREG_gc; // enable protected register change
   CLK.CTRL = CLK_SCLKSEL_PLL_gc; // switch to PLL
   //
   // set up pins 
   //
   button_ctrl = PORT_OPC_PULLUP_gc;
   set(led_port.OUT,led_pin);
   set(led_port.DIR,led_pin);
   //
   // set up ATP ports
   //
   ATP_init(ports,4,40); // name,number,buffer size
   ATP_setup(ports,0,D,0,1,2,3); // number,vport,BI,TI,BO,TO
   ATP_setup(ports,1,D,4,5,6,7);
   ATP_setup(ports,2,C,1,0,3,2);
   ATP_setup(ports,3,C,5,4,6,7);
   //ATP_setup(ports,3,A,2,3,1,0);
   //
   // delay for power-on transients
   //
   _delay_ms(10);
   //
   // initial conditions
   //
   ATP_push_byte_out(ports,0,0b11001010);
   //
   // event loop
   //
   while (1) {
      ATP_scan(ports,0);
      ATP_scan(ports,1);
      ATP_scan(ports,2);
      ATP_scan(ports,3);
      }
   }

