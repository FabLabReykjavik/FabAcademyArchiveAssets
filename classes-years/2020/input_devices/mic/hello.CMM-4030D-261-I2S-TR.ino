//
// hello.CMM-4030D-261-I2S-TR.ino
//
// CMM-4030D-261-I2S-TR I2C microphone hello-world
//    10 MHz clock, 24 bit samples, 115200 baud data
//
// Neil Gershenfeld 12/27/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define nsamples 200
#define nstart 2000
#define EN PIN2_bm
#define SCK PIN1_bm
#define DS PIN0_bm
#define WS PIN3_bm
#define set(port,pin) (port |= pin)
#define clr(port,pin) (port &= (~pin))
#define sck_low clr(VPORTA.OUT,SCK);
#define sck_high set(VPORTA.OUT,SCK);
#define ws_low_sck_low VPORTA.OUT = VPORTA.OUT&(~(WS|SCK));
#define ws_high_sck_low VPORTA.OUT = (VPORTA.OUT&(~SCK))|WS;
#define clock_delay tmp = VPORTA.IN;tmp = VPORTA.IN;tmp = VPORTA.IN;tmp = VPORTA.IN;
#define read_bit(byte_arg) byte_arg = (byte_arg<<1)+(VPORTB.IN&DS);tmp = VPORTA.IN;tmp = VPORTA.IN;tmp = VPORTA.IN;
// 2x(1+4) 100us instructions = 1 MHz bits = 1/(64+2) = 15 kHz samples

uint8_t tmp,sample,hibyte,midbyte,lobyte,hibytes[nsamples],midbytes[nsamples],lobytes[nsamples];
uint16_t count;

void setup() {
   Serial.begin(115200);
   set(VPORTA.OUT,WS);
   set(VPORTA.DIR,WS);
   set(VPORTA.OUT,SCK);
   set(VPORTA.DIR,SCK);
   set(VPORTA.OUT,EN);
   set(VPORTA.DIR,EN);
   }

void read_sample(uint8_t sample) {
   //
   // left channel (not connected)
   //
   ws_low_sck_low;
   clock_delay;
   sck_high; // up 1
   clock_delay;
   sck_low; // down 1
   clock_delay;
   sck_high; // up 2
   clock_delay;
   sck_low; // down 2
   clock_delay;
   sck_high; // up 3
   clock_delay;
   sck_low; // down 3
   clock_delay;
   sck_high; // up 4
   clock_delay;
   sck_low; // down 4
   clock_delay;
   sck_high; // up 5
   clock_delay;
   sck_low; // down 5
   clock_delay;
   sck_high; // up 6
   clock_delay;
   sck_low; // down 6
   clock_delay;
   sck_high; // up 7
   clock_delay;
   sck_low; // down 7
   clock_delay;
   sck_high; // up 8
   clock_delay;
   sck_low; // down 8
   clock_delay;
   sck_high; // up 9
   clock_delay;
   sck_low; // down 9
   clock_delay;
   sck_high; // up 10
   clock_delay;
   sck_low; // down 10
   clock_delay;
   sck_high; // up 11
   clock_delay;
   sck_low; // down 11
   clock_delay;
   sck_high; // up 12
   clock_delay;
   sck_low; // down 12
   clock_delay;
   sck_high; // up 13
   clock_delay;
   sck_low; // down 13
   clock_delay;
   sck_high; // up 14
   clock_delay;
   sck_low; // down 14
   clock_delay;
   sck_high; // up 15
   clock_delay;
   sck_low; // down 15
   clock_delay;
   sck_high; // up 16
   clock_delay;
   sck_low; // down 16
   clock_delay;
   sck_high; // up 17
   clock_delay;
   sck_low; // down 17
   clock_delay;
   sck_high; // up 18
   clock_delay;
   sck_low; // down 18
   clock_delay;
   sck_high; // up 19
   clock_delay;
   sck_low; // down 19
   clock_delay;
   sck_high; // up 20
   clock_delay;
   sck_low; // down 20
   clock_delay;
   sck_high; // up 21
   clock_delay;
   sck_low; // down 21
   clock_delay;
   sck_high; // up 22
   clock_delay;
   sck_low; // down 22
   clock_delay;
   sck_high; // up 23
   clock_delay;
   sck_low; // down 23
   clock_delay;
   sck_high; // up 24
   clock_delay;
   sck_low; // down 24
   clock_delay;
   sck_high; // up 25
   clock_delay;
   sck_low; // down 25
   clock_delay;
   sck_high; // up 26
   clock_delay;
   sck_low; // down 26
   clock_delay;
   sck_high; // up 27
   clock_delay;
   sck_low; // down 27
   clock_delay;
   sck_high; // up 28
   clock_delay;
   sck_low; // down 28
   clock_delay;
   sck_high; // up 29
   clock_delay;
   sck_low; // down 29
   clock_delay;
   sck_high; // up 30
   clock_delay;
   sck_low; // down 30
   clock_delay;
   sck_high; // up 31
   clock_delay;
   sck_low; // down 31
   clock_delay;
   sck_high; // up 32
   clock_delay;
   //
   // read right channel
   //
   ws_high_sck_low;
   clock_delay;
   sck_high; // up 1
   clock_delay;
   sck_low; // down 1
   clock_delay;
   sck_high; // up 2
   read_bit(hibyte);
   sck_low; // down 2
   clock_delay;
   sck_high; // up 3
   read_bit(hibyte);
   sck_low; // down 3
   clock_delay;
   sck_high; // up 4
   read_bit(hibyte);
   sck_low; // down 4
   clock_delay;
   sck_high; // up 5
   read_bit(hibyte);
   sck_low; // down 5
   clock_delay;
   sck_high; // up 6
   read_bit(hibyte);
   sck_low; // down 6
   clock_delay;
   sck_high; // up 7
   read_bit(hibyte);
   sck_low; // down 7
   clock_delay;
   sck_high; // up 8
   read_bit(hibyte);
   sck_low; // down 8
   clock_delay;
   sck_high; // up 9
   read_bit(hibyte);
   sck_low; // down 9
   clock_delay;
   sck_high; // up 10
   read_bit(midbyte);
   sck_low; // down 10
   clock_delay;
   sck_high; // up 11
   read_bit(midbyte);
   sck_low; // down 11
   clock_delay;
   sck_high; // up 12
   read_bit(midbyte);
   sck_low; // down 12
   clock_delay;
   sck_high; // up 13
   read_bit(midbyte);
   sck_low; // down 13
   clock_delay;
   sck_high; // up 14
   read_bit(midbyte);
   sck_low; // down 14
   clock_delay;
   sck_high; // up 15
   read_bit(midbyte);
   sck_low; // down 15
   clock_delay;
   sck_high; // up 16
   read_bit(midbyte);
   sck_low; // down 16
   clock_delay;
   sck_high; // up 17
   read_bit(midbyte);
   sck_low; // down 17
   clock_delay;
   sck_high; // up 18
   read_bit(lobyte);
   sck_low; // down 18
   clock_delay;
   sck_high; // up 19
   read_bit(lobyte);
   sck_low; // down 19
   clock_delay;
   sck_high; // up 20
   read_bit(lobyte);
   sck_low; // down 20
   clock_delay;
   sck_high; // up 21
   read_bit(lobyte);
   sck_low; // down 21
   clock_delay;
   sck_high; // up 22
   read_bit(lobyte);
   sck_low; // down 22
   clock_delay;
   sck_high; // up 23
   read_bit(lobyte);
   sck_low; // down 23
   clock_delay;
   sck_high; // up 24
   read_bit(lobyte);
   sck_low; // down 24
   clock_delay;
   sck_high; // up 25
   read_bit(lobyte);
   sck_low; // down 25
   clock_delay;
   sck_high; // up 26
   read_bit(lobyte);
   sck_low; // down 26
   clock_delay;
   sck_high; // up 27
   clock_delay;
   sck_low; // down 27
   clock_delay;
   sck_high; // up 28
   clock_delay;
   sck_low; // down 28
   clock_delay;
   sck_high; // up 29
   clock_delay;
   sck_low; // down 29
   clock_delay;
   sck_high; // up 30
   clock_delay;
   sck_low; // down 30
   clock_delay;
   sck_high; // up 31
   clock_delay;
   sck_low; // down 31
   clock_delay;
   sck_high; // up 32
   clock_delay;
   sck_low; // down 32
   clock_delay;
   sck_high;
   hibytes[sample] = hibyte;
   midbytes[sample] = midbyte;
   lobytes[sample] = lobyte;
   }

void loop() {
   //
   // dummy reads for microphone to wake up and settle
   //
   for (count = 0; count < nstart; ++count)
      read_sample(0);
   //
   // read samples
   //
   for (sample = 0; sample < nsamples; ++sample)
      read_sample(sample);
   //
   // send framing
   //
   Serial.write(1);
   Serial.write(2);
   Serial.write(3);
   Serial.write(4);
   //
   // send samples
   //
   for (sample = 0; sample < nsamples; ++sample) {
      Serial.write(lobytes[sample]);
      Serial.write(midbytes[sample]);
      Serial.write(hibytes[sample]);
      }
   }
