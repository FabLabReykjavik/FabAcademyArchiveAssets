//
// hello.LEDs.t412.ino
//
// series-parallel LEDs ATtiny412 hello-world
//
// Neil Gershenfeld 12/30/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define max_threshold 100
#define max_cycle 10000
#define num_cycles 5
#define LED_pin PIN6_bm
#define LED_port VPORTA.OUT
#define set(port,pin) (port |= pin)
#define clr(port,pin) (port &= (~pin))

void setup() {
   PORTA.DIRSET = LED_pin;
   }

uint16_t cycle,cycles,threshold;

void loop() {
   while(1) {
      for (threshold = 0; threshold < max_threshold; ++threshold) {
         for (cycles = 0; cycles < num_cycles; ++cycles) {
            for (cycle = 0; cycle < threshold; ++cycle)
               set(LED_port,LED_pin);
            for (cycle = threshold; cycle < max_cycle; ++cycle)
               clr(LED_port,LED_pin);
            }
         }
      for (threshold = max_threshold; threshold > 0; --threshold) {
         for (cycles = 0; cycles < num_cycles; ++cycles) {
            for (cycle = 0; cycle < threshold; ++cycle)
               set(LED_port,LED_pin);
            for (cycle = threshold; cycle < max_cycle; ++cycle)
               clr(LED_port,LED_pin);
            }
         }
      }
   }
