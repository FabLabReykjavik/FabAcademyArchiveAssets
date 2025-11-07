//
// hello.DRV8251A.RP2040.ino
//    DRV8251A RP2040 stepper hello-world
//
// Neil Gershenfeld 6/21/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//
// use printf library
//
#include <LibPrintf.h>
//
// step parameters
//
#define pwm_range 65535
#define pwm_frequency 1e6 
#define micro_steps 100
#define min_step_delay_us 50
#define max_step_delay_us 1000
float amplitude = 0.3*pwm_range;
#define step_count 20000
//
// define pins
//
#define A0 D1
#define A1 D2
#define B0 D5
#define B1 D6
#define VA D0
#define VB D4
//
// global trig tables
//
float sin_table[micro_steps],cos_table[micro_steps];
//
// setup
//
void setup() {
  Serial.begin();
  analogWriteFreq(pwm_frequency);
  analogWriteRange(pwm_range);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(B0,OUTPUT);
  pinMode(B1,OUTPUT);
  digitalWriteFast(A0,LOW);
  digitalWriteFast(A1,LOW);
  digitalWriteFast(B0,LOW);
  digitalWriteFast(B1,LOW);
  analogWrite(VA,0);
  analogWrite(VB,0);
  for (int i = 0; i < micro_steps; ++i) {
    sin_table[i] = sin(2*PI*i/micro_steps);
    cos_table[i] = cos(2*PI*i/micro_steps);
    }
  analogReadResolution(12);
  }
//
// microstep and return current
//
float microstep(int cycle_count,float amplitude,int step_delay_us) {
  int step = cycle_count%micro_steps;
  int A = amplitude*sin_table[step];
  int B = amplitude*cos_table[step];
  //
  if (A > 0) {
    digitalWriteFast(A0,HIGH);
    digitalWriteFast(A1,LOW);
    }
  else {
    digitalWriteFast(A0,LOW);
    digitalWriteFast(A1,HIGH);
    }
  analogWrite(VA,abs(A));
  //
  if (B > 0) {
    digitalWriteFast(B0,HIGH);
    digitalWriteFast(B1,LOW);
    }
  else {
    digitalWriteFast(B0,LOW);
    digitalWriteFast(B1,HIGH);
    }
  analogWrite(VB,abs(B));
  //
  delayMicroseconds(step_delay_us);
  //
  int ADC = analogRead(A3);
  float current = (3.3*ADC/4095)/(1575e-6*499);
  return current;
  }
//
// nothing to do in first core
//
void loop() {
  }
//
// main loop, in 2nd core
//
void loop1() {
  float current;
  //
  // ramp up forward
  //
  current = 0;
  for (int step = 0; step < step_count/2; ++step) {
    float x = (2.0*step)/step_count;
    int delay = (1-x)*max_step_delay_us+x*min_step_delay_us;
    current += microstep(step,amplitude,delay);
    } 
  printf("%f amps average\n",2*current/step_count);
  //
  // ramp down forward
  //
  current = 0;
  for (int step = step_count/2; step < step_count; ++step) {
    float x = (2.0*(step-step_count/2))/step_count;
    int delay = (1-x)*min_step_delay_us+x*max_step_delay_us;
    current += microstep(step,amplitude,delay);
    } 
  printf("%f amps average\n",2*current/step_count);
  //
  // ramp up reverse
  //
  current = 0;
  for (int step = 0; step < step_count/2; ++step) {
    float x = (2.0*step)/step_count;
    int delay = (1-x)*max_step_delay_us+x*min_step_delay_us;
    current += microstep(step_count-step,amplitude,delay);
    } 
  printf("%f amps average\n",2*current/step_count);
  //
  // ramp down reverse
  //
  current = 0;
  for (int step = step_count/2; step < step_count; ++step) {
    float x = (2.0*(step-step_count/2))/step_count;
    int delay = (1-x)*min_step_delay_us+x*max_step_delay_us;
    current += microstep(step_count-step,amplitude,delay);
    } 
  printf("%f amps average\n",2*current/step_count);
  }


