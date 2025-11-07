//
// hello.ICS-43434.RP2040.ino
//    XIAO RP2040 ICS-43434 I2S mic hello-world
//
// Neil Gershenfeld 11/5/24
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <I2S.h>

#define clock 1
#define frame (clock+1)
#define data 4
#define sample_bits 24
#define sample_rate 44100
#define nsamples 1000

I2S i2s(INPUT);

void setup() {
  Serial.begin(115200);
  i2s.setBCLK(clock);
  i2s.setDATA(data);
  i2s.setBitsPerSample(sample_bits);
  i2s.begin(sample_rate);
  }

void loop() {
  int32_t sample,samples[nsamples];
  for (int i = 0; i < nsamples; ++i) {
    samples[i] = i2s.read();
    sample = i2s.read();
    }
  Serial.println("frame");
  for (int i = 0; i < nsamples; ++i) {
    Serial.println(samples[i]);
    }
  }
