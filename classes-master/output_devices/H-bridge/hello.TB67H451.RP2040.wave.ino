//
// hello.TB67H451.RP2040.wave.ino
//    XIAO RP2040 TB67H451 H-bridge audio waveform synthesis hello-world
//    overclock at 250 MHz 
//
// Neil Gershenfeld 11/21/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define sample_frequency 44100
#define sample_period_us 1e6/sample_frequency
#define sample_scale 0.75
#define pwm_frequency 50000
#define pwm_range 65535
#define t0 0.05
#define t1 0.15
#define t2 0.3
#define t3 0.6
#define notes_length 24

// 0 ... 40	= Middle C, 49 = A440
float pitches[] = {0,27.50000,29.13524,30.86771,32.70320,34.64783,36.70810,38.89087,41.20344,43.65353,46.24930,48.99943,51.91309,55.00000,58.27047,61.73541,65.40639,69.29566,73.41619,77.78175,82.40689,87.30706,92.49861,97.99886,103.8262,110.0000,116.5409,123.4708,130.8128,138.5913,146.8324,155.5635,164.8138,174.6141,184.9972,195.9977,207.6523,220.0000,233.0819,246.9417,261.6256,277.1826,293.6648,311.1270,329.6276,349.2282,369.9944,391.9954,415.3047,440.0000,466.1638,493.8833,523.2511,554.3653,587.3295,622.2540,659.2551,698.4565,739.9888,783.9909,830.6094,880.0000,932.3275,987.7666,1046.502,1108.731,1174.659,1244.508,1318.510,1396.913,1479.978,1567.982,1661.219,1760.000,1864.655,1975.533,2093.005,2217.461,2349.318,2489.016,2637.020,2793.826,2959.955,3135.963,3322.438,3520.000,3729.310,3951.066,4186.009};

struct {
  int key;
  float time;
  } notes[] = {
  {45,t2},
  {0,t0},
  {49,t2},
  {0,t0},
  {45,t2},
  {0,t0},
  {52,t2},
  {0,t0},
  {45,t2},
  {0,t0},
  {57,t2},
  {0,t0},
  {56,t1},
  {54,t1},
  {52,t1},
  {54,t1},
  {52,t1},
  {50,t1},
  {49,t1},
  {50,t1},
  {49,t1},
  {47,t1},
  {45,t2},
  {0,t3}
  };

void setup() {
  Serial.begin();
  analogWriteFreq(pwm_frequency);
  analogWriteRange(pwm_range);
  }

uint32_t tstart = 0;
void loop() {
  uint32_t t,tend,duty;
  for (int note = 0; note < notes_length; ++note) {
    float pitch = pitches[notes[note].key];
    tend = tstart+notes[note].time*1e6;
    while (t < tend) {
      t = micros();
      if ((t-tstart) > sample_period_us) {
        tstart = t;
        duty = pwm_range*sin(2*PI*pitch*t/1e6);
        if (duty > 0) {
          analogWrite(0,0);
          analogWrite(1,sample_scale*duty);
          }
        else {
          analogWrite(0,-sample_scale*duty);
          analogWrite(1,0);
          }
        }
      }
    }
  }
