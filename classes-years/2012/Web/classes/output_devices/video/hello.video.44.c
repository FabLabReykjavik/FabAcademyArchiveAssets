//
// hello.video.44.c
//
// NTSC/RS170 video hello-world
//
// set lfuse to 0x7E for 20 MHz xtal
//
// Neil Gershenfeld
// 11/14/10
//
// (c) Massachusetts Institute of Technology 2010
// Permission granted for experimental and personal use;
// license for commercial sale available from MIT.
//

#include <avr/io.h>
#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set

#define DAC_port PORTA
#define DAC_direction DDRA
#define pin_499 (1 << PA7) // 499 ohm DAC pin
#define pin_1000 (1 << PA6) // 1000 ohm DAC pin

#define black pin_1000 // .306V for 499,1k DAC
#define gray pin_499 // .613V for 499,1k DAC
#define white (pin_499 | pin_1000) // .919V for 499,1k DAC
#define out_zero PORTA = 0; // output 0V
#define out_black PORTA = black; // output black level
#define horizontal_sync count(0,82); // 4.7us horizontal sync
#define front_porch count(0,50); // off-screen beginning of scan line
#define scan_line count(4,142); // 58.8us complete scan line
#define back_porch count(0,70); // off-screen end of scan line
#define blank_line \
   out_zero; \
   horizontal_sync; \
   out_black; \
   scan_line; // blank horizontal scan line
#define sync_line \
   out_black; \
   horizontal_sync; \
   out_zero; \
   scan_line; // vertical sync line

void count(uint8_t hi, uint8_t lo) {
   //
   // delay counter
   //
   OCR1AH = hi; // set match count
   OCR1AL = lo; //
   TCNT1H = 0; // clear counter
   TCNT1L = 0; //
   TIFR1 = (1 << OCF1A); // clear count match flag
   while (0 == (TIFR1 & (1 << OCF1A)))
      ;
   }

int main(void) {
   //
   // main
   //
   static uint8_t frame,offset,row,col,end;
   static uint8_t pattern[125];
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   // 
   // set counter prescaler to /1
   //
   TCCR1B = (1 << CS10);
   //
   // initialize DAC pins
   //
   clear(DAC_port, pin_499);
   output(DAC_direction, pin_499);
   clear(DAC_port, pin_1000);
   output(DAC_direction, pin_1000);
   //
   // store test pattern
   //
   col = 0;
   do {
      pattern[col++] = black;
      pattern[col++] = gray;
      pattern[col++] = black;
      pattern[col++] = white;
      } while (col < 125);
   //
   // main loop
   //
   while (1) {
      //
      // loop over offsets
      //
      for (offset = 0; offset < 4; ++offset) {
         //
         // loop over frames
         //
         for (frame = 0; frame < 20; ++frame) {
            // 
            // 242 63.5us horizontal scan lines
            //
            for (row = 0; row < 242; ++row) {
               out_zero;
      	      horizontal_sync;
      	      out_black;
      	      front_porch;
      	      // 
      	      // column loop
      	      //
               col = offset + (row / 20);
               end = 87 + offset + (row / 20);
               do {
                  PORTA = pattern[col++];
                  } while (col < end);
       	      out_black;
      	      back_porch;
               }
            //
            // 5 black lines
            //
            for (row = 0; row < 5; ++row) {
               out_zero;
      	      horizontal_sync;
               out_black;
      	      scan_line;
               }
            //
            // 3 vertical sync lines
            //
            for (row = 0; row < 3; ++row)
               sync_line;
            //
            // 12 black lines
            //
            for (row = 0; row < 12; ++row) {
               out_zero;
      	      horizontal_sync;
               out_black;
      	      scan_line;
               }
            //
            // end of 60Hz frame loop
            //
            }
         }
      }
   }
