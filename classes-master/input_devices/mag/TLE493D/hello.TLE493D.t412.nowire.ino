//
// hello.TLE493D.t412.nowire.ino
//    TLE493D vector magnetometer tiny412 hello-world
//    no I2C library version
//
// Neil Gershenfeld 3/29/25
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#define address 0x35
#define SDA PIN_PA1
#define SCL PIN_PA2
#define I2C_delay() delayMicroseconds(5)

unsigned char ret,data[10];

void SCL_write(unsigned char bit, unsigned char SCL_pin) {
   //
   // write SCL bit
   //
   if (bit == 0) {
      pinMode(SCL_pin,OUTPUT);
      digitalWrite(SCL_pin,LOW);
      }
   else {
      pinMode(SCL_pin,INPUT);
      while (digitalRead(SCL_pin) == LOW); // check for clock stretching
      }
   }

void SDA_write(unsigned char bit, unsigned char SDA_pin) {
   //
   // write SDA bit
   //
   if (bit == 0) {
      pinMode(SDA_pin,OUTPUT);
      digitalWrite(SDA_pin,LOW);
      }
   else
      pinMode(SDA_pin,INPUT);
   }

void I2C_init(unsigned char SCL_pin, unsigned char SDA_pin) {
   //
   // initialize I2C lines
   //
   SDA_write(1,SDA_pin);
   SCL_write(1,SCL_pin);
   } 

unsigned char I2C_master_write_byte(unsigned char byte,
   unsigned char SCL_pin, unsigned char SDA_pin) {
   //
   // master write I2C byte
   //
   unsigned char bit;
   unsigned char ret = 0;
   //
   // loop over bits
   //
   for (bit = 0; bit < 8; ++bit) {
      if ((byte & 0x80) == 0)
         SDA_write(0,SDA);
      else
         SDA_write(1,SDA);
      I2C_delay();
      SCL_write(1,SCL);
      I2C_delay();
      SCL_write(0,SCL);
      byte <<= 1;
      }
   //
   // check for ACK
   //
   I2C_delay();
   SDA_write(1,SDA);
   SCL_write(1,SCL);
   I2C_delay();
   if (digitalRead(SDA) != LOW) {
      //
      // no ACK, return 1
      //
      ret = 1;	
      }
   //
   // yes ACK, return 0
   //
   SCL_write(0,SCL);
   I2C_delay();
   return ret;
   }	

unsigned char I2C_master_write(unsigned char* data, unsigned char nbytes, unsigned char secondary_address,
   unsigned char SCL_pin, unsigned char SDA_pin) {
   //
   // I2C master write
   //
   unsigned char index,secondary_address_write;
   unsigned char ret = 0;
   //
   // send start
   //
   SDA_write(0,SDA);
   I2C_delay();
   SCL_write(0,SCL);	
   //
   // send secondary address
   //
   secondary_address_write = secondary_address << 1;
   if (I2C_master_write_byte(secondary_address_write, SCL_pin, SDA_pin) != 0)
      //
      // no ACK, return 1
      //
      ret = 1;
   //
   // loop over bytes
   //
   for (index = 0; index < nbytes; ++index) {
      ret = I2C_master_write_byte(data[index], SCL_pin, SDA_pin);
      if (ret != 0)
         //
         // no ACK, return 1
         //
         ret = 1;
      //
      // yes ACK, continue
      //	
      }
   //
   // send stop
   //
   SDA_write(0,SDA);
   I2C_delay();
   SCL_write(1,SCL);
   I2C_delay();
   SDA_write(1,SDA);
   I2C_delay();
   return ret;
   }

void I2C_master_read_byte(unsigned char* data, unsigned char index, unsigned char nbytes,
   unsigned char SCL_pin, unsigned char SDA_pin) {
   //
   // master read I2C byte
   //
   unsigned char byte,bit;
   SDA_write(1,SDA);
   byte = 0;
   //
   // loop over bits
   //
   for (bit = 0; bit < 8; ++bit)  {
      I2C_delay();
      SCL_write(1,SCL);
      I2C_delay();
      if (digitalRead(SDA) != LOW)
         byte |= (1 << (7-bit));
      SCL_write(0,SCL);
      }
   data[index] = byte;
   if (index < (nbytes-1)) {
      //
      // not done, send ACK
      //
      SDA_write(0,SDA);
      I2C_delay();
      SCL_write(1,SCL);
      I2C_delay();
      SCL_write(0,SCL);
      SDA_write(1,SDA);
      }
   else {
      //
      // done, send NACK
      //
      SDA_write(1,SDA);
      I2C_delay();
      SCL_write(1,SCL);
      I2C_delay();
      SCL_write(0,SCL);
      I2C_delay();
      }
   }	

unsigned char I2C_master_read(unsigned char* data, unsigned char nbytes, unsigned char secondary_address,
   unsigned char SCL_pin, unsigned char SDA_pin) {
   //
   // I2C master read
   //
   unsigned char index,secondary_address_read;
   unsigned char ret = 0;
   //
   // send start
   //
   SDA_write(0,SDA);
   I2C_delay();
   SCL_write(0,SCL);	
   I2C_delay();
   //
   // send secondary address
   //
   secondary_address_read = (secondary_address << 1) + 1;
   if (I2C_master_write_byte(secondary_address_read, SCL_pin, SDA_pin) == 1)
      //
      // no ACK, return 1
      //
      ret = 1;
   //
   // loop over bytes
   //
   for (index = 0; index < nbytes; ++index)
      I2C_master_read_byte(data, index, nbytes, SCL_pin, SDA_pin);
   //
   // send stop
   //
   SCL_write(1,SCL);
   I2C_delay();
   SDA_write(1,SDA);
   I2C_delay();
   return ret;
   }	

void setup() {
   //
   // start serial and I2C
   //
   Serial.begin(115200);
   I2C_init(SCL,SDA);
   //
   // reset TLE493D
   //
   data[0] = 0xFF;
   ret = I2C_master_write(data,1,address,SCL,SDA);
   data[0] = 0xFF;
   ret = I2C_master_write(data,1,address,SCL,SDA);
   data[0] = 0x00;
   ret = I2C_master_write(data,1,address,SCL,SDA);
   data[0] = 0x00;
   ret = I2C_master_write(data,1,address,SCL,SDA);
   //
   // configure TLE493D
   //
   data[0] = 0x10;
   data[1] = 0x28;
      // config register 0x10
      // ADC trigger on read after register 0x05
      // short-range sensitivity
   data[2] = 0x15;
      // mode register 0x11
      // 1-byte read protocol
      // interrupt disabled
      // master controlled mode
   ret = I2C_master_write(data,3,address,SCL,SDA);
   }

void loop() {
   //
   // read registers 0x00-0x05
   //
   ret = I2C_master_write(data,0,address,SCL,SDA);
   ret = I2C_master_read(data,6,address,SCL,SDA);
   //
   // send framing
   //
   Serial.write(1);
   Serial.write(2);
   Serial.write(3);
   Serial.write(4);
   //
   // send data
   //
   Serial.write(data,6);
   }
