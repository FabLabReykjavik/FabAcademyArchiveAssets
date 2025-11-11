//
// hello.speaker-registers.D11C.ino
//
// speaker D11C hello-world, registers version
//
// Neil Gershenfeld 11/6/22
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define pwm_top 100

void setup() {
   //
   // set up PWM
   //
   GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(4) | // use clock generator 4
      GCLK_GENCTRL_GENEN | // enable clock generator
      GCLK_GENCTRL_SRC_DFLL48M | // 48MHz source
      GCLK_GENCTRL_IDC; // 50-50 duty cycle
   while (GCLK->STATUS.bit.SYNCBUSY); // sync
   GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) | // divide by 1
      GCLK_GENDIV_ID(4); // apply to GCLK4
   while (GCLK->STATUS.bit.SYNCBUSY); // sync
   GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | // enable clock control
      GCLK_CLKCTRL_GEN_GCLK4 | // select GCLK4
      GCLK_CLKCTRL_ID_TCC0; // connect to TCC0
   while (GCLK->STATUS.bit.SYNCBUSY); // sync
   TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV1_Val); // divide by 1
   TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM; // normal PWM
   while (TCC0->SYNCBUSY.bit.WAVE); // sync
   TCC0->PER.reg = pwm_top; // PWM top
   while (TCC0->SYNCBUSY.bit.PER); // sync
   TCC0->CC[1].reg = 0; // compare WO[1] for PA05
   while (TCC0->SYNCBUSY.bit.CC1); // sync
   PORT->Group[PORTA].DIRSET.reg = PORT_PA05; // set pin output
   PORT->Group[PORTA].OUTCLR.reg = PORT_PA05; // set pin low
   PORT->Group[PORTA].PINCFG[5].reg |= PORT_PINCFG_PMUXEN; // enable port mux
   PORT->Group[PORTA].PMUX[2].reg = PORT_PMUX_PMUXO_F; // connect pin 2*2+1 to TCC0
   TCC0->CTRLA.reg |= (TCC_CTRLA_ENABLE); // enable TCC
   while (TCC0->SYNCBUSY.bit.ENABLE); // sync
   }

void loop() {
   //
   // 1 kHz square wave
   //
   uint32_t frequency_Hz = 1000;
   uint32_t delay_us = 1e6/(frequency_Hz*2);
   uint32_t pwm_compare = pwm_top/2;
   float ontime_sec = 1;
   uint32_t cycle,cycle_count = frequency_Hz*ontime_sec;
   for (cycle = 0; cycle < cycle_count; ++cycle) {
      TCC0->CC[1].reg = pwm_compare;
      delayMicroseconds(delay_us);
      TCC0->CC[1].reg = 0;
      delayMicroseconds(delay_us);
      }
   //
   // 500 Hz square wave
   //
   frequency_Hz = 500;
   delay_us = 1e6/(frequency_Hz*2);
   pwm_compare = pwm_top/2;
   ontime_sec = 1;
   cycle,cycle_count = frequency_Hz*ontime_sec;
   for (cycle = 0; cycle < cycle_count; ++cycle) {
      TCC0->CC[1].reg = pwm_compare;
      delayMicroseconds(delay_us);
      TCC0->CC[1].reg = 0;
      delayMicroseconds(delay_us);
      }
   //
   // 1 kHz square wave, reduce volume
   //
   frequency_Hz = 1000;
   delay_us = 1e6/(frequency_Hz*2);
   pwm_compare = pwm_top/4;
   ontime_sec = 1;
   cycle,cycle_count = frequency_Hz*ontime_sec;
   for (cycle = 0; cycle < cycle_count; ++cycle) {
      TCC0->CC[1].reg = pwm_compare;
      delayMicroseconds(delay_us);
      TCC0->CC[1].reg = 0;
      delayMicroseconds(delay_us);
      }
   //
   // 500 Hz square wave, reduce volume
   //
   frequency_Hz = 500;
   delay_us = 1e6/(frequency_Hz*2);
   pwm_compare = pwm_top/4;
   ontime_sec = 1;
   cycle,cycle_count = frequency_Hz*ontime_sec;
   for (cycle = 0; cycle < cycle_count; ++cycle) {
      TCC0->CC[1].reg = pwm_compare;
      delayMicroseconds(delay_us);
      TCC0->CC[1].reg = 0;
      delayMicroseconds(delay_us);
      }
   }
