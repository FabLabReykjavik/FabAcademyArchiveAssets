//
// hello.ALS31313.t412.c
//    vector magnetometer tiny412 hello-world
//
// Neil Gershenfeld 12/19/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
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

#define serial_port_direction VPORTA.DIR
#define serial_port_out VPORTA.OUT
#define serial_port_in VPORTA.IN
#define serial_pin_out PIN6_bm
#define serial_pin_in PIN7_bm

#define address 96

#define I2C_delay() _delay_us(10)
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

char I2C_master_write_byte(unsigned char byte) {
   //
   // master write I2C byte
   //
   unsigned char bit;
   //
   // loop over bits
   //
   for (bit = 0; bit < 8; ++bit) {
      if ((byte & 0x80) == 0)
         SDA_write(0);
      else
         SDA_write(1);
      SCL_write(1);
      I2C_delay();
      SCL_write(0);
      I2C_delay();
      byte <<= 1;
      }
   //
   // check for ACK
   //
   SDA_write(1);
   SCL_write(1);
   I2C_delay();
   if (pin_test(SDA_pins,SDA_pin) != 0) {
      //
      // no ACK, return 1
      //
      ;//return 2;	
      }
   //
   // yes ACK, return 0
   //
   SCL_write(0);
   I2C_delay();
   return 0;
   }	

char I2C_master_write(unsigned char* data, unsigned char nbytes, unsigned char secondary_address) {
   //
   // I2C master write
   //
   unsigned char index,ret,secondary_address_write;
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
   secondary_address_write = secondary_address << 1;
   if (I2C_master_write_byte(secondary_address_write) != 0)
      //
      // no ACK, return 1
      //
      ;//return 3;
   //
   // loop over bytes
   //
   for (index = 0; index < nbytes; ++index) {
      ret = I2C_master_write_byte(data[index]);
      if (ret != 0)
         //
         // no ACK, return 1
         //
         ;//break;
      //
      // yes ACK, continue
      //	
      }
   //
   // send stop
   //
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
      SCL_write(1);
      I2C_delay();
      if (pin_test(SDA_pins,SDA_pin) != 0)
         byte |= (1 << (7-bit));
      SCL_write(0);
      I2C_delay();
      }
   data[index] = byte;
   if (index < (nbytes-1)) {
      //
      // not done, send ACK
      //
      SDA_write(0);
      SCL_write(1);
      I2C_delay();
      SCL_write(0);
      SDA_write(1);
      I2C_delay();
      }
   else {
      //
      // done, send NACK
      //
      SDA_write(1);
      SCL_write(1);
      I2C_delay();
      SCL_write(0);
      I2C_delay();
      }
   }	

char I2C_master_read(unsigned char* data, unsigned char nbytes, unsigned char secondary_address) {
   //
   // I2C master read
   //
   unsigned char index,secondary_address_read;
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
      return 4;
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
   return 0;
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
   uint8_t ret,data[8];
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
   //
   // main loop
   //
   while (1) {
      //
      // read fields
      //
      data[0] = 0x28;
      ret = I2C_master_write(data,1,address);
      ret = I2C_master_read(data,8,address);
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
      put_char(&serial_port_out,serial_pin_out,data[5]);
      put_char(&serial_port_out,serial_pin_out,data[6]);
      put_char(&serial_port_out,serial_pin_out,data[7]);
      }
   }
