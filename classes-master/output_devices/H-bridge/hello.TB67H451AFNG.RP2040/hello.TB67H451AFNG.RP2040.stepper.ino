
// 250 MHz

#define pwm_range 65535
#define micro_steps 100
#define step_delay_us 100
#define step_count 20000
#define pwm_frequency 1e6

float amplitude = 0.7*pwm_range;

#define A0 D1
#define A1 D2
#define B0 D4
#define B1 D5
#define VA D0
#define VB D3

float sin_table[micro_steps],cos_table[micro_steps];

void setup() {
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
  }

void microstep(int count,int amplitude) {
  int step = count%micro_steps;
  int A = amplitude*sin_table[step];
  int B = amplitude*cos_table[step];
  if (A > 0) {
    digitalWriteFast(A0,HIGH);
    digitalWriteFast(A1,LOW);
    }
  else {
    digitalWriteFast(A0,LOW);
    digitalWriteFast(A1,HIGH);
    }
  analogWrite(VA,abs(A));
  if (B > 0) {
    digitalWriteFast(B0,HIGH);
    digitalWriteFast(B1,LOW);
    }
  else {
    digitalWriteFast(B0,LOW);
    digitalWriteFast(B1,HIGH);
    }
  analogWrite(VB,abs(B));
  }

void loop() {
  }

void loop1() {
  for (int step = 0; step < step_count; ++step) {
    microstep(step,amplitude);
    delayMicroseconds(step_delay_us);
    } 
  for (int step = step_count; step > 0; --step) {
    microstep(step,amplitude);
    delayMicroseconds(step_delay_us);
    } 
  }
