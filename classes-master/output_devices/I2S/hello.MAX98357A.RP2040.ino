//
// hello.MAX98357A.RP2040.ino
//    XIAO RP2040 MAX98357A I2S amp hello-world
//
// Neil Gershenfeld 11/3/24
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
#define amplitude 10000.0
#define sample_rate 22050.0
#define sample_bits 16
#define notes_length 138
#define note_length 0.18

// 0 ... 40	= Middle C, 49 = A440
float pitches[] = {0,27.50000,29.13524,30.86771,32.70320,34.64783,36.70810,38.89087,
  41.20344,43.65353,46.24930,48.99943,51.91309,55.00000,58.27047,61.73541,65.40639,
  69.29566,73.41619,77.78175,82.40689,87.30706,92.49861,97.99886,103.8262,110.0000,
  116.5409,123.4708,130.8128,138.5913,146.8324,155.5635,164.8138,174.6141,184.9972,
  195.9977,207.6523,220.0000,233.0819,246.9417,261.6256,277.1826,293.6648,311.1270,
  329.6276,349.2282,369.9944,391.9954,415.3047,440.0000,466.1638,493.8833,523.2511,
  554.3653,587.3295,622.2540,659.2551,698.4565,739.9888,783.9909,830.6094,880.0000,
  932.3275,987.7666,1046.502,1108.731,1174.659,1244.508,1318.510,1396.913,1479.978,
  1567.982,1661.219,1760.000,1864.655,1975.533,2093.005,2217.461,2349.318,2489.016,
  2637.020,2793.826,2959.955,3135.963,3322.438,3520.000,3729.310,3951.066,4186.009};

// from J.S. Bach Invention No. 8
int notes[] = {
  0,0,0,45,45,49,49,45,45,52,52,45,45,
  57,57,56,54,52,54,52,50,49,50,49,47,
  45,45,49,49,52,52,49,49,57,57,52,52,
  61,64,62,64,61,64,62,64,61,64,62,64,
  57,61,59,61,57,61,59,61,57,61,59,61,
  54,57,56,57,54,57,56,57,54,57,56,57,
  51,51,47,47,54,54,51,51,57,57,54,54,
  59,61,59,57,56,57,56,54,52,54,52,50,
  49,49,54,52,51,52,51,49,47,49,47,45,
  44,45,44,42,40,40,52,51,52,52,44,44,
  45,45,52,52,44,44,52,52,42,42,51,51,
  52,52,52,52,0,0,0
};

I2S i2s(OUTPUT);

void setup() {
  i2s.setBCLK(clock);
  i2s.setDATA(data);
  i2s.setBitsPerSample(sample_bits);
  i2s.begin(sample_rate);
  }

void loop() {
  float t=0,tend,duration,amp;
  float dt = 1/sample_rate;
  int16_t sample;
  //
  // loop over notes
  //
  for (int note = 1; note < (notes_length-1); ++note) {
    float pitch = pitches[notes[note]];
    duration = note_length;
    tend = t+duration;
    //
    // loop over samples
    //
    while (t < tend) {
      t += dt;
      //
      // taper amplitude at start and end on new notes to eliminate pops
      //
      if ((tend-t) > ((0.9*duration)) && (notes[note] != notes[note-1]))
        amp = amplitude*((1-(tend-t)/duration)/(1-0.9));
      else if (((tend-t) < (0.1*duration)) && (notes[note] != notes[note+1]))
        amp = amplitude*(((tend-t)/duration)/0.1);
      else
          amp = amplitude;
      sample = amp*sin(2*PI*t*pitch);
      //
      // output left and right samples 
      //
      i2s.write(sample);
      i2s.write(sample);
      }
    }
  }
