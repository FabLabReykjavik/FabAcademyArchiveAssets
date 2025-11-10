//
// hello.DRV8428-D11C.ino
//
// DRV8428-D11C stepper hello-world
//
// Neil Gershenfeld 5/30/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define LEDA 4
#define LEDC 2
#define EN 5
#define DIR 8
#define STEP 9
#define M1 14
#define M0 15
#define NSTEPS 1000
#define DELAYHIGH 10
#define DELAYLOW 1000
#define BLINK 100

void setup() {
   digitalWrite(LEDA,HIGH);
   pinMode(LEDA,OUTPUT);
   digitalWrite(LEDC,LOW);
   pinMode(LEDC,OUTPUT);
   digitalWrite(EN,HIGH);
   pinMode(EN,OUTPUT);
   digitalWrite(STEP,LOW);
   pinMode(STEP,OUTPUT);
   digitalWrite(DIR,LOW);
   pinMode(DIR,OUTPUT);
   }

void blink_off() {
   digitalWrite(LEDA,LOW);
   delay(BLINK);
   digitalWrite(LEDA,HIGH);
   }

void do_steps() {
   digitalWrite(DIR,HIGH);
   for (int i = 0; i < NSTEPS; ++i) {
      digitalWrite(STEP,HIGH);
      delayMicroseconds(DELAYHIGH);
      digitalWrite(STEP,LOW);
      delayMicroseconds(DELAYLOW);
      }
   digitalWrite(DIR,LOW);
   for (int i = 0; i < NSTEPS; ++i) {
      digitalWrite(STEP,HIGH);
      delayMicroseconds(DELAYHIGH);
      digitalWrite(STEP,LOW);
      delayMicroseconds(DELAYLOW);
      }
   }

void step_2() {
   digitalWrite(M0,HIGH);
   digitalWrite(M1,LOW);
   pinMode(M0,OUTPUT);
   pinMode(M1,OUTPUT);
   }

void step_4() {
   digitalWrite(M0,LOW);
   digitalWrite(M1,HIGH);
   pinMode(M0,OUTPUT);
   pinMode(M1,OUTPUT);
   }

void step_8() {
   digitalWrite(M0,HIGH);
   digitalWrite(M1,HIGH);
   pinMode(M0,OUTPUT);
   pinMode(M1,OUTPUT);
   }

void step_16() {
   digitalWrite(M1,HIGH);
   pinMode(M0,INPUT);
   pinMode(M1,OUTPUT);
   }

void step_32() {
   digitalWrite(M0,LOW);
   pinMode(M0,OUTPUT);
   pinMode(M1,INPUT);
   }

void step_128() {
   pinMode(M0,INPUT);
   pinMode(M1,INPUT);
   }

void step_256() {
   digitalWrite(M0,HIGH);
   pinMode(M0,OUTPUT);
   pinMode(M1,INPUT);
   }

void loop() {
   blink_off();
   step_2();
   do_steps();
   blink_off();
   step_4();
   do_steps();
   blink_off();
   step_8();
   do_steps();
   blink_off();
   step_16();
   do_steps();
   blink_off();
   step_32();
   do_steps();
   blink_off();
   step_128();
   do_steps();
   blink_off();
   step_256();
   do_steps();
   }
