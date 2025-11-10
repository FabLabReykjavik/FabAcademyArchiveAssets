//
// ATP.XMEGA.E.h
//    ATP XMEGA E header
//
// Neil Gershenfeld
// 12/13/15
//
// (c) Massachusetts Institute of Technology 2015
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

/*

logical states
   T(oken)
    B(it)
   00 x empty
   01 o occupied
   10 0 0
   11 1 1

send handshake
     x -- x
   0,1 -> x
   0,1 <- o
     x -> o
     x <- x

receive handshake
     x -- x
     x <- 0,1
     o -> 0,1
     o <- x
     x -> x

send/receive signals
      send  receive
    O x11xx xxoox
   TO _--__ _____
   BO _--__ __--_
   TI _____ _--__
   BI __--_ _--__
    I xxoox x11xx

swap handshake
     x -- x
   0,1 -> x
   0,1 <- 0,1
     o -> 0,1
     o <- o
     x -> o
     x <- x

simultaneous swap handshake
     x -- x
   0,1 <> 0,1
     o <> o
     x <> x

missed simultaneous swap handshake
     x -- x
   0,1 <> 0,1
     o <> x
     x -> x

port states
   out   out
      in   in
   00 00 x x empty
   00 01 x o send acknowledge
   00 10 x 0 receive available
   00 11 x 1 receive available
   01 00 o x receive acknowledge
   01 01 o o swap accept
   01 10 o 0 receive accept
   01 11 o 1 receive accept
   10 00 0 x send available
   10 01 0 o send accept
   10 10 0 0 swap available
   10 11 0 1 swap available
   11 00 1 x send available
   11 01 1 o send accept
   11 10 1 0 swap available
   11 11 1 1 swap available
*/

//
// includes
//
#include <avr/io.h>
#include <util/delay.h>
//
// virtual ports
//
#define VPORTA VPORT0 // pre-mapped virtual port A
#define VPORTC VPORT1 // pre-mapped virtual port C
#define VPORTD VPORT2 // pre-mapped virtual port D
#define VPORTR VPORT3 // pre-mapped virtual port R
//
// logical states
//              T(oken)
//               B(it)
#define ATP_x 0b00
#define ATP_o 0b01
#define ATP_0 0b10
#define ATP_1 0b11
//
// port states
//          out  out
//           in    in
#define ATP_xx 0b0000
#define ATP_xo 0b0001
#define ATP_x0 0b0010
#define ATP_x1 0b0011
#define ATP_ox 0b0100
#define ATP_oo 0b0101
#define ATP_o0 0b0110
#define ATP_o1 0b0111
#define ATP_0x 0b1000
#define ATP_0o 0b1001
#define ATP_00 0b1010
#define ATP_01 0b1011
#define ATP_1x 0b1100
#define ATP_1o 0b1101
#define ATP_10 0b1110
#define ATP_11 0b1111
//
// ATP port type
//
typedef struct {
   unsigned char number;
   VPORT_t *vport;
   unsigned char bi; // bit in pin
   unsigned char bo; // bit out pin
   unsigned char ti; // token in pin
   unsigned char to; // token out pin
   unsigned char mask; // pin bit mask
   unsigned char size; // byte buffer size
   struct {
      struct {
         unsigned char value;
         unsigned char count;
         } bit;
      struct {
         unsigned char value;
         unsigned char count;
         } byte;
      } in; // input port
   struct {
      struct {
         unsigned char value;
         unsigned char count;
         } bit;
      struct {
         unsigned char value;
         unsigned char count;
         } byte;
      } out; // output port
   } ATP_port;
//
// macros
//
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
//
// ATP_init
//    initialize ATP data structure
//
#define ATP_init(name,number,bufsize)\
   static ATP_port name[number]; {\
      unsigned char i;\
      for (i = 0; i < number; ++i)\
         name[i].size = bufsize;\
      }
//
// ATP_setup
//    set up ATP port
//
#define ATP_setup(name,id,letter,bipin,tipin,bopin,topin)\
   name[id].vport = &VPORT ## letter;\
   name[id].bi = (1 << bipin);\
   name[id].ti = (1 << tipin);\
   name[id].bo = (1 << bopin);\
   name[id].to = (1 << topin);\
   name[id].in.byte.value = 0;\
   name[id].in.byte.count = 0;\
   name[id].out.byte.value = 0;\
   name[id].out.byte.count = 0;\
   name[id].mask = (1<<bopin)|(1<<topin)|(1<<bipin)|(1<<tipin);\
   name[id].vport->OUT &= ~name[id].bo;\
   name[id].vport->OUT &= ~name[id].to;\
   name[id].vport->DIR |= name[id].bo;\
   name[id].vport->DIR |= name[id].to;\
   PORT ## letter.PIN ## bipin ## CTRL = PORT_OPC_PULLDOWN_gc;\
   PORT ## letter.PIN ## tipin ## CTRL = PORT_OPC_PULLDOWN_gc;
//
// ATP_input
//    read state from ATP port
//
#define ATP_input(ports,id) ({\
   static unsigned char in;\
   in = ports[id].vport->IN;\
   (((in & ports[id].bi)?0b0001:0)|((in & ports[id].ti)?0b0010:0)|((in & ports[id].bo)?0b0100:0)|((in & ports[id].to)?0b1000:0));\
   })
//
// ATP_output
//    write state to ATP port
//
#define ATP_output(ports,id,state) {\
   switch (state) {\
      case ATP_x:\
         ports[id].vport->OUT = (ports[id].vport->OUT & ~ports[id].mask) | 0;\
         break;\
      case ATP_o:\
         ports[id].vport->OUT = (ports[id].vport->OUT & ~ports[id].mask) | (ports[id].bo);\
         break;\
      case ATP_0:\
         ports[id].vport->OUT = (ports[id].vport->OUT & ~ports[id].mask) | (ports[id].to);\
         break;\
      case ATP_1:\
         ports[id].vport->OUT = (ports[id].vport->OUT & ~ports[id].mask) | (ports[id].to|ports[id].bo);\
         break;\
      }\
   }
//
// ATP_put_bit_out
//    put ATP bit out
//
#define ATP_put_bit_out(ports,id) {\
   if (ports[id].out.bit.count == 1) {\
      if (ports[id].out.bit.value == 0) {\
         ATP_output(ports,id,ATP_0);}\
      else {\
         ATP_output(ports,id,ATP_1);}\
      }\
   }
//
// ATP_accept_bit_out
//    accept ATP bit out
//
#define ATP_accept_bit_out(ports,id) {\
   ports[id].out.bit.count = 0;\
   ATP_process_bit_out(ports,id);\
   }
//
// ATP_get_bit_in
//     get ATP bit in
//
#define ATP_get_bit_in(ports,id,bitvalue) {\
   ports[id].in.bit.value = bitvalue;\
   }
//
// ATP_accept_bit_in
//    accept ATP bit in
//
#define ATP_accept_bit_in(ports,id) {\
   ports[id].in.bit.count = 1;\
   ATP_process_bit_in(ports,id);\
   }
//
// ATP_push_bit_out
//    push ATP bit out
//
#define ATP_push_bit_out(ports,id,bitvalue) {\
   ports[id].out.bit.value = bitvalue;\
   ports[id].out.bit.count = 1;\
   }
//
// ATP_pop_bit_in
//    pop ATP bit in
//
#define ATP_pop_bit_in(ports,id) {\
   ports[id].in.bit.value = 0;\
   ports[id].in.bit.count = 0;\
   }
//
// ATP_push_byte_out
//    push ATP byte out
//
#define ATP_push_byte_out(ports,id,bytevalue) {\
   ports[id].out.bit.value = (bytevalue >> 7) & 1;\
   ports[id].out.bit.count = 1;\
   ports[id].out.byte.value = bytevalue;\
   ports[id].out.byte.count = 7;\
   }
//
// ATP_pop_byte_in
//    pop ATP byte in
//
#define ATP_pop_byte_in(ports,id) {\
   ports[id].in.byte.value = 0;\
   ports[id].in.byte.count = 0;\
   }
//
// ATP_push_bit_byte_in
//    push bit on ATP byte in
#define ATP_push_bit_byte_in(ports,id) {\
   ports[id].in.byte.value = (ports[id].in.byte.value << 1) | ports[id].in.bit.value;\
   ports[id].in.byte.count += 1;\
   }
//
// ATP_pop_bit_byte_out
//    pop bit on ATP byte out
//
#define ATP_pop_bit_byte_out(ports,id) {\
   if (ports[id].out.byte.count > 0) {\
      ports[id].out.byte.count -= 1;\
      ports[id].out.bit.value = (ports[id].out.byte.value >> ports[id].out.byte.count) & 1;\
      ports[id].out.bit.count = 1;\
      }\
   }
//
// ATP_check_state
//    check for ATP state
//
#define ATP_check_state(ports,id,state) {\
   if (ATP_input(ports,id) != state) {\
      break;\
      }\
   }
//
// ATP_scan
//    scan port
//
#define ATP_scan(ports,id) {\
   switch (ATP_input(ports,id)) {\
      case ATP_xx:\
         ATP_check_state(ports,id,ATP_xx);\
         ATP_put_bit_out(ports,id);\
         break;\
      case ATP_0o:\
         ATP_check_state(ports,id,ATP_0o);\
         ATP_output(ports,id,ATP_x);\
         ATP_accept_bit_out(ports,id);\
         break;\
      case ATP_1o:\
         ATP_check_state(ports,id,ATP_1o);\
         ATP_output(ports,id,ATP_x);\
         ATP_accept_bit_out(ports,id);\
         break;\
      case ATP_x0:\
         ATP_check_state(ports,id,ATP_x0);\
         ATP_output(ports,id,ATP_o);\
         ATP_get_bit_in(ports,id,0);\
         break;\
      case ATP_x1:\
         ATP_check_state(ports,id,ATP_x1);\
         ATP_output(ports,id,ATP_o);\
         ATP_get_bit_in(ports,id,1);\
         break;\
      case ATP_ox:\
         ATP_check_state(ports,id,ATP_ox);\
         ATP_output(ports,id,ATP_x);\
         ATP_accept_bit_in(ports,id);\
         break;\
      case ATP_00:\
         ATP_check_state(ports,id,ATP_00);\
         ATP_output(ports,id,ATP_o);\
         ATP_get_bit_in(ports,id,0);\
         break;\
      case ATP_01:\
         ATP_check_state(ports,id,ATP_01);\
         ATP_output(ports,id,ATP_o);\
         ATP_get_bit_in(ports,id,1);\
         break;\
      case ATP_10:\
         ATP_check_state(ports,id,ATP_10);\
         ATP_output(ports,id,ATP_o);\
         ATP_get_bit_in(ports,id,0);\
         break;\
      case ATP_11:\
         ATP_check_state(ports,id,ATP_11);\
         ATP_output(ports,id,ATP_o);\
         ATP_get_bit_in(ports,id,1);\
         break;\
      case ATP_oo:\
         ATP_check_state(ports,id,ATP_oo);\
         ATP_output(ports,id,ATP_x);\
         ATP_accept_bit_out(ports,id);\
         ATP_accept_bit_in(ports,id);\
         break;\
      }\
   }

