//
// hello.mag.D11C.ino
//
// Hall effect D11C hello-world
//
// Neil Gershenfeld 11/2/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define nsamples 100

void setup() {
   //
   // start serial
   //
   SerialUSB.begin(0);
   //
   // set up ADC
   //    assumes Arduino core has set up ADC clock
   //
   ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTVCC1;
      // reference 1/2 VDDANA
   ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1;
      // average 1 sample
   ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV128 |
      ADC_CTRLB_RESSEL_10BIT;
      // prescaler divide by 128, 10 bit result
   ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_DIV2 |
      ADC_INPUTCTRL_MUXNEG_GND | ADC_INPUTCTRL_MUXPOS_PIN0;
      // gain 1/2, ground negative, AIN[0] (pin 13) positive
    PORT->Group[0].DIRCLR.reg = PORT_PA02;
      // input on PA02 (pin 13)
   PORT->Group[0].PINCFG[2].reg |= PORT_PINCFG_PMUXEN;
      // enable multiplexer for PA02
   PORT->Group[0].PMUX[1].reg = PORT_PMUX_PMUXE_B;
      // function B (analog input) on PA02
   while (ADC->STATUS.bit.SYNCBUSY) {};
      // wait for sync
   ADC->CTRLA.bit.ENABLE = true;
      // enable
   }

void loop() {
   uint32_t result = 0;
   //
   // accumulate readings
   //
   for (int count = 0; count < nsamples; ++count) {
      while (ADC->STATUS.bit.SYNCBUSY) {}; // wait for sync
      ADC->SWTRIG.bit.START = true; // start conversion
      while (ADC->INTFLAG.bit.RESRDY == 0) {}; // wait for ready
      ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY; // clear flag
      result += ADC->RESULT.reg; // accumulate result
   }
   //
   // send framing
   //
   SerialUSB.write(1);
   SerialUSB.write(2);
   SerialUSB.write(3);
   SerialUSB.write(4);
   //
   // send reading
   // 
   SerialUSB.write(result & 255);
   SerialUSB.write((result >> 8) & 255);
   SerialUSB.write((result >> 16) & 255);
   }
