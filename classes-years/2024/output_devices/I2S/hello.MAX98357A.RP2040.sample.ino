//
// hello.MAX98357A.RP2040.sample.ino
//    XIAO RP2040 MAX98537A I2S amp audio sample hello-world
//
// Neil Gershenfeld 11/7/24
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <I2S.h>
#include "hello.h"

#define clock 1
#define frame (clock+1)
#define data 4
#define sample_frequency 44100
#define sample_length 40876
#define sample_bits 16
#define sample_scale 0.1
#define fade 10000

I2S i2s(OUTPUT);

void setup() {
  i2s.setBCLK(clock);
  i2s.setDATA(data);
  i2s.setBitsPerSample(sample_bits);
  i2s.begin(sample_frequency);
  }

void loop() {
  int32_t sample;
  //
  // loop over samples
  //
  for (int i = 0; i < sample_length; ++i) {
    //
    // output left and right channels, fading out
    //
    if (i > (sample_length-fade))
      sample = ((sample_length-i-1)/(fade-1))*sample_scale*hello[i];
    else
      sample = sample_scale*hello[i];
    i2s.write(sample);
    i2s.write(sample);
    }
  delay(500);
  }