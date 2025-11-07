//
// hello.TLE493D.t412.c
//    TLE493D vector magnetometer tiny412 hello-world
//
// Neil Gershenfeld 12/22/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//
//

#include <avr/io.h>
#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define bit_delay_time 8.5 // bit delay for 115200 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set

#define address 0x35

#define serial_port_direction VPORTA.DIR
#define serial_port_out VPORTA.OUT
#define serial_port_in VPORTA.IN
#define serial_pin_out PIN6_bm
#define serial_pin_in PIN7_bm

#define VDD_pin PIN3_bm
#define VDD_pins VPORTA.IN
#define VDD_port VPORTA.OUT
#define VDD_direction VPORTA.DIR

#define I2C_delay() _delay_us(5)
#define SCL_pin PIN2_bm
#define SCL_pins VPORTA.IN
#define SCL_port VPORTA.OUT
#define SCL_direction VPORTA.DIR
#define SDA_pin PIN1_bm
#define SDA_pins VPORTA.IN
#define SDA_port VPORTA.OUT
#define SDA_direction VPORTA.DIR

void SCL_write(char bit) {
   //
   // write SCL bit
   //
   if (bit == 0) {
      output(SCL_direction,SCL_pin);
      clear(SCL_port,SCL_pin);
      }
   else {
      input(SCL_direction,SCL_pin);
      while (pin_test(SCL_pins,SCL_pin) == 0); // check for clock stretching
      }
   }

void SDA_write(char bit) {
   //
   // write SDA bit
   //
   if (bit == 0) {
      output(SDA_direction,SDA_pin);
      clear(SDA_port,SDA_pin);
      }
   else
      input(SDA_direction,SDA_pin);
   }

void I2C_init() {
   //
   // initialize I2C lines
   //
   SDA_write(1);
   SCL_write(1);
   } 

unsigned char I2C_master_write_byte(unsigned char byte) {
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
         SDA_write(0);
      else
         SDA_write(1);
      I2C_delay();
      SCL_write(1);
      I2C_delay();
      SCL_write(0);
      byte <<= 1;
      }
   //
   // check for ACK
   //
   I2C_delay();
   SDA_write(1);
   SCL_write(1);
   I2C_delay();
   if (pin_test(SDA_pins,SDA_pin) != 0) {
      //
      // no ACK, return 1
      //
      ret = 1;	
      }
   //
   // yes ACK, return 0
   //
   SCL_write(0);
   I2C_delay();
   return ret;
   }	

unsigned char I2C_master_write(unsigned char* data, unsigned char nbytes, unsigned char secondary_address) {
   //
   // I2C master write
   //
   unsigned char index,secondary_address_write;
   unsigned char ret = 0;
   //
   // send start
   //
   SDA_write(0);
   I2C_delay();
   SCL_write(0);	
   //
   // send secondary address
   //
   secondary_address_write = secondary_address << 1;
   if (I2C_master_write_byte(secondary_address_write) != 0)
      //
      // no ACK, return 1
      //
      ret = 1;
   //
   // loop over bytes
   //
   for (index = 0; index < nbytes; ++index) {
      ret = I2C_master_write_byte(data[index]);
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
   SDA_write(0);
   I2C_delay();
   SCL_write(1);
   I2C_delay();
   SDA_write(1);
   I2C_delay();
   return ret;
   }

void I2C_master_read_byte(unsigned char* data, unsigned char index, unsigned char nbytes) {
   //
   // master read I2C byte
   //
   unsigned char byte,bit;
   SDA_write(1);
   byte = 0;
   //
   // loop over bits
   //
   for (bit = 0; bit < 8; ++bit)  {
      I2C_delay();
      SCL_write(1);
      I2C_delay();
      if (pin_test(SDA_pins,SDA_pin) != 0)
         byte |= (1 << (7-bit));
      SCL_write(0);
      }
   data[index] = byte;
   if (index < (nbytes-1)) {
      //
      // not done, send ACK
      //
      SDA_write(0);
      I2C_delay();
      SCL_write(1);
      I2C_delay();
      SCL_write(0);
      SDA_write(1);
      }
   else {
      //
      // done, send NACK
      //
      SDA_write(1);
      I2C_delay();
      SCL_write(1);
      I2C_delay();
      SCL_write(0);
      I2C_delay();
      }
   }	

unsigned char I2C_master_read(unsigned char* data, unsigned char nbytes, unsigned char secondary_address) {
   //
   // I2C master read
   //
   unsigned char index,secondary_address_read;
   unsigned char ret = 0;
   //
   // send start
   //
   SDA_write(0);
   I2C_delay();
   SCL_write(0);	
   I2C_delay();
   //
   // send secondary address
   //
   secondary_address_read = (secondary_address << 1) + 1;
   if (I2C_master_write_byte(secondary_address_read) == 1)
      //
      // no ACK, return 1
      //
      ret = 1;
   //
   // loop over bytes
   //
   for (index = 0; index < nbytes; ++index)
      I2C_master_read_byte(data,index,nbytes);
   //
   // send stop
   //
   SCL_write(1);
   I2C_delay();
   SDA_write(1);
   I2C_delay();
   return ret;
   }	

void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte) {
   //
   // read character into rxbyte on pins pin
   //    assumes line driver (inverts bits)
   //
   *rxbyte = 0;
   while (pin_test(*pins,pin))
      //
      // wait for start bit
      //
      ;
   //
   // delay to middle of first data bit
   //
   half_bit_delay();
   bit_delay();
   //
   // unrolled loop to read data bits
   //
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 0);
   else
      *rxbyte |= (0 << 0);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 1);
   else
      *rxbyte |= (0 << 1);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 2);
   else
      *rxbyte |= (0 << 2);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 3);
   else
      *rxbyte |= (0 << 3);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 4);
   else
      *rxbyte |= (0 << 4);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 5);
   else
      *rxbyte |= (0 << 5);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 6);
   else
      *rxbyte |= (0 << 6);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 7);
   else
      *rxbyte |= (0 << 7);
   //
   // wait for stop bit
   //
   bit_delay();
   half_bit_delay();
   }

void put_char(volatile unsigned char *port, unsigned char pin, char txchar) {
   //
   // send character in txchar on port pin
   //    assumes line driver (inverts bits)
   //
   // start bit
   //
   clear(*port,pin);
   bit_delay();
   //
   // unrolled loop to write data bits
   //
   if bit_test(txchar,0)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,1)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,2)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,3)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,4)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,5)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,6)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,7)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   //
   // stop bit
   //
   set(*port,pin);
   bit_delay();
   //
   // char delay
   //
   bit_delay();
   }

int main(void) {
   //
   // main
   //
   unsigned char ret,data[10];
   //
   // unprotect clock and turn off prescalar (20 MHz)
   //
   CPU_CCP = CCP_IOREG_gc;
   CLKCTRL.MCLKCTRLB = 0;
   //
   // initialize pins
   //
   set(serial_port_out,serial_pin_out);
   set(serial_port_direction,serial_pin_out);
   I2C_init();
   //
   // power TLE493D
   //
   set(VDD_port,VDD_pin);
   set(VDD_direction,VDD_pin);
   //
   // reset TLE493D
   //
   data[0] = 0xFF;
   ret = I2C_master_write(data,1,address);
   data[0] = 0xFF;
   ret = I2C_master_write(data,1,address);
   data[0] = 0x00;
   ret = I2C_master_write(data,1,address);
   data[0] = 0x00;
   ret = I2C_master_write(data,1,address);
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
   ret = I2C_master_write(data,3,address);
   //
   // main loop
   //
   while (1) {
      //
      // read registers 0x00-0x05
      //
      ret = I2C_master_write(data,0,address);
      ret = I2C_master_read(data,6,address);
      //
      // send framing
      //
      put_char(&serial_port_out,serial_pin_out,1);
      put_char(&serial_port_out,serial_pin_out,2);
      put_char(&serial_port_out,serial_pin_out,3);
      put_char(&serial_port_out,serial_pin_out,4);
      //
      // send data
      //
      put_char(&serial_port_out,serial_pin_out,data[0]);
      put_char(&serial_port_out,serial_pin_out,data[1]);
      put_char(&serial_port_out,serial_pin_out,data[2]);
      put_char(&serial_port_out,serial_pin_out,data[3]);
      put_char(&serial_port_out,serial_pin_out,data[4]);
      put_char(&serial_port_out,serial_pin_out,data[5]);
      }
   }
