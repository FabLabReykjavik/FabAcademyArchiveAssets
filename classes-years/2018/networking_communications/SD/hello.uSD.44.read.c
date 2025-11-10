//
//
// hello.uSD.44.read.c
//
// 19200 baud microSD hello-world
//    assumes FAT32 SDHC/XC
//
// Neil Gershenfeld 11/26/18
//
// (c) Massachusetts Institute of Technology 2018
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 49.5 // bit delay for 19200 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay
#define SPI_delay() _delay_us(2.5) // SPI delay

#define serial_port PORTA
#define serial_direction DDRA
#define serial_pins PINA
#define serial_pin_in (1 << PA2)
#define serial_pin_out (1 << PA3)

#define CS_port PORTA
#define CS_direction DDRA
#define CS_pin (1 << PA7)
#define SCK_port PORTB
#define SCK_direction DDRB
#define SCK_pin (1 << PB1)
#define MOSI_port PORTB
#define MOSI_direction DDRB
#define MOSI_pin (1 << PB2)
#define MISO_port PORTB
#define MISO_direction DDRB
#define MISO_pin (1 << PB0)
#define MISO_pins PINB

//
// put_char
//   send character in txchar
//    assumes line driver (inverts bits)
//    assume use of serial_port, serial_pin_out
//
void put_char(char txchar) {
   //
   // start bit
   //
   clear(serial_port,serial_pin_out);
   bit_delay();
   //
   // unrolled loop to write data bits
   //
   if bit_test(txchar,0)
      set(serial_port,serial_pin_out);
   else
      clear(serial_port,serial_pin_out);
   bit_delay();
   if bit_test(txchar,1)
      set(serial_port,serial_pin_out);
   else
      clear(serial_port,serial_pin_out);
   bit_delay();
   if bit_test(txchar,2)
      set(serial_port,serial_pin_out);
   else
      clear(serial_port,serial_pin_out);
   bit_delay();
   if bit_test(txchar,3)
      set(serial_port,serial_pin_out);
   else
      clear(serial_port,serial_pin_out);
   bit_delay();
   if bit_test(txchar,4)
      set(serial_port,serial_pin_out);
   else
      clear(serial_port,serial_pin_out);
   bit_delay();
   if bit_test(txchar,5)
      set(serial_port,serial_pin_out);
   else
      clear(serial_port,serial_pin_out);
   bit_delay();
   if bit_test(txchar,6)
      set(serial_port,serial_pin_out);
   else
      clear(serial_port,serial_pin_out);
   bit_delay();
   if bit_test(txchar,7)
      set(serial_port,serial_pin_out);
   else
      clear(serial_port,serial_pin_out);
   bit_delay();
   //
   // stop bit
   //
   set(serial_port,serial_pin_out);
   bit_delay();
   //
   // char delay
   //
   bit_delay();
   }
//
// put_string
//    print a null-terminated string
//    assumes use of serial_port, serial_pin_out
//
void put_string(char *str) {
   static int index;
   index = 0;
   do {
      put_char(str[index]);
      ++index;
      } while (str[index] != 0);
   }
//
// put_flash_string
//    print a null-terminated string from flash
//    assumes use of serial_port, serial_pin_out
//
void put_flash_string(const char *string) {
   static int index;
   index = 0;
   do {
      put_char(pgm_read_byte(string+index));
      ++index;
      } while (pgm_read_byte(string+index) != 0);
   }
//
// put_char_string
//    print a string
//    assumes use of serial_port, serial_pin_out
//
void put_char_string(uint8_t *string, uint16_t length) {
   static int index;
   for (index = 0; index < length; ++index) {
      put_char(string[index]);
      }
   }
//
// put_hex_char
//    put a char in hex
void put_hex_char(uint8_t chr) {
   static char hex[] = "0123456789ABCDEF";
      put_char(hex[(chr >> 4) & 0x0F]);
      put_char(hex[chr & 0x0F]);
   }
//
// put_hex_string
//    print a string in hex
//    assumes use of serial_port, serial_pin_out
//
void put_hex_string(uint8_t *string, uint16_t length) {
   static int index;
   for (index = 0; index < length; ++index) {
      put_hex_char(string[index]);
      }
   }
//
// SPI_write
//    write an SPI character and return the response
//
unsigned char SPI_write(uint8_t chr) {
   static unsigned char bit,ret;
   ret = 0;
   //
   // bit loop
   //
   for (bit = 0; bit < 8; ++bit) {
      clear(SCK_port,SCK_pin);
      if (chr & (1 << (7-bit)))
         set(MOSI_port,MOSI_pin);
      else
         clear(MOSI_port,MOSI_pin);
      SPI_delay();
      set(SCK_port,SCK_pin);
      SPI_delay();
      if pin_test(MISO_pins,MISO_pin)
         ret |= (1 << (7-bit));
      }
   //
   // finish
   //
   return ret;
   }
//
// SD_command
//    write an SD command and return the response
//
void SD_command(uint8_t command,uint32_t argument,uint8_t CRC,uint8_t *result) {
   clear(CS_port,CS_pin);
   SPI_write(command);
   SPI_write((argument >> 24) & 0xFF);
   SPI_write((argument >> 16) & 0xFF);
   SPI_write((argument >> 8) & 0xFF);
   SPI_write(argument & 0xFF);
   SPI_write(CRC);
   result[0] = SPI_write(0xFF);
   result[1] = SPI_write(0xFF);
   result[2] = SPI_write(0xFF);
   result[3] = SPI_write(0xFF);
   result[4] = SPI_write(0xFF);
   result[5] = SPI_write(0xFF);
   result[6] = SPI_write(0xFF);
   result[7] = SPI_write(0xFF);
   set(CS_port,CS_pin);
   }
//
// SD_read
//    read size bytes at offset in sector into buffer
//
void SD_read(uint32_t sector,uint16_t offset,uint8_t *buffer,uint16_t size) {
   static uint8_t chr;
   static uint16_t index,count;
   //
   // start read
   //
   clear(CS_port,CS_pin);
   //
   // send CMD17
   //
   SPI_write(0x51);
   SPI_write((sector >> 24) & 0xFF);
   SPI_write((sector >> 16) & 0xFF);
   SPI_write((sector >> 8) & 0xFF);
   SPI_write(sector & 0xFF);
   SPI_write(0);
   //
   // loop until 0xFE data token
   //
   while (1) {
      chr = SPI_write(0xFF);
      if (chr == 0xFE)
         break;
      }
   //
   // read up to offset
   //
   for (index = 0; index < offset; ++index) {
      chr = SPI_write(0xFF);
      }
   //
   // read from offset
   //
   count = 0;
   for (index = offset; index < (offset+size); ++index) {
      buffer[count] = SPI_write(0xFF);
      count += 1;
      }
   //
   // read up to sector
   //
   for (index = (offset+size); index < 512; ++index) {
      chr = SPI_write(0xFF);
      }
   //
   // read checksum
   //
   chr = SPI_write(0xFF);
   chr = SPI_write(0xFF);
   //
   // finish read
   //
   chr = SPI_write(0xFF);
   set(CS_port,CS_pin);
   }
//
// main
//
int main(void) {
   //
   // main
   //
   static uint8_t count,sectors_per_cluster,FATs,attribute,result[8],buffer[50];
   static uint16_t bytes_per_sector,reserved_sectors,offset,
      file_cluster_low,file_cluster_hi,buffer_length,sector_count;
   static uint32_t partition,FAT_sectors,fat_sector,root_cluster,
      root_sector,file_length,file_cluster,file_sector,chars_read;
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize output pins
   //
   set(serial_port,serial_pin_out);
   output(serial_direction,serial_pin_out);
   set(CS_port,CS_pin);
   output(CS_direction,CS_pin);
   clear(MOSI_port,MOSI_pin);
   output(MOSI_direction,MOSI_pin);
   set(SCK_port,SCK_pin);
   output(SCK_direction,SCK_pin);
   set(MISO_port,MISO_pin); //turn on pull-up
   input(MISO_direction,MISO_pin);
   //
   // put card in SPI mode
   //
   set(MOSI_port,MOSI_pin);
   set(CS_port,CS_pin);
   for (count = 0; count < 80; ++count) {
      set(SCK_port,SCK_pin);
      SPI_delay();
      clear(SCK_port,SCK_pin);
      SPI_delay();
      }
   //
   // CMD0: reset and enter idle state
   //    should return 0x01
   //
   put_flash_string(PSTR("\r\nreset: 0x"));
   SD_command(0x40,0,0x95,result);
   put_hex_string(result,8);
   //
   // CMD8: send interface condition, set SDHC
   //    should return 0x01000001AA
   //
   put_flash_string(PSTR("\r\nset interface: 0x"));
   SD_command(0x48,0x000001AA,0x87,result);
   put_hex_string(result,8);
   //
   // initialization loop
   //
   put_flash_string(PSTR("\r\ninitialization:"));
   while (1) {
      //
      // CMD55: application command follows
      //    should return 0x01
      //
      put_flash_string(PSTR("\r\n   application command: "));
      SD_command(0x77,0,0,result);
      put_hex_string(result,8);
      //
      // ACMD41: initialize the card
      //    should return 0x00 when ready
      //
      put_flash_string(PSTR("\r\n   initialize card: 0x"));
      SD_command(0x69,0x40000000,0,result);
      put_hex_string(result,8);
      //
      // check if done
      //
      if (result[1] == 0)
         break;
      }
   //
   // read the first partition table
   //
   put_flash_string(PSTR("\r\nread first partition table:"));
   SD_read(0,0x1BE,buffer,32);
   put_flash_string(PSTR("\r\n   type: 0x")); // 0x0B or 0x0C for FAT32
   put_hex_string(buffer+4,1);
   put_flash_string(PSTR("\r\n   first sector: 0x"));
   put_hex_string(buffer+8,4);
   memcpy(&partition,buffer+8,4);
   //
   // read the first partition block
   //
   put_flash_string(PSTR("\r\nread first partition block:"));
   SD_read(partition,0,buffer,50);
   //
   memcpy(&bytes_per_sector,buffer+11,2);
   put_flash_string(PSTR("\r\n   bytes per sector: 0x"));
   put_hex_char((bytes_per_sector >> 8) & 0xFF);
   put_hex_char(bytes_per_sector);
   //
   memcpy(&sectors_per_cluster,buffer+13,1);
   put_flash_string(PSTR("\r\n   sectors per cluster: 0x"));
   put_hex_char(sectors_per_cluster);
   //
   memcpy(&reserved_sectors,buffer+14,2);
   put_flash_string(PSTR("\r\n   reserved sectors: 0x"));
   put_hex_char((reserved_sectors >> 8) & 0xFF);
   put_hex_char(reserved_sectors & 0xFF);
   //
   fat_sector = partition+reserved_sectors;
   //
   memcpy(&FATs,buffer+16,1);
   put_flash_string(PSTR("\r\n   FATs: 0x"));
   put_hex_char(FATs & 0xFF);
   //
   memcpy(&FAT_sectors,buffer+36,4);
   put_flash_string(PSTR("\r\n   FAT sectors: 0x"));
   put_hex_char((FAT_sectors >> 24) & 0xFF);
   put_hex_char((FAT_sectors >> 16) & 0xFF);
   put_hex_char((FAT_sectors >> 8) & 0xFF);
   put_hex_char(FAT_sectors  & 0xFF);
   //
   memcpy(&root_cluster,buffer+44,4);
   put_flash_string(PSTR("\r\n   root cluster: 0x"));
   put_hex_char((root_cluster >> 24) & 0xFF);
   put_hex_char((root_cluster >> 16) & 0xFF);
   put_hex_char((root_cluster >> 8) & 0xFF);
   put_hex_char(root_cluster & 0xFF);
   //
   // read the root directory
   //
   root_sector = partition+reserved_sectors+(FATs*FAT_sectors);
   put_flash_string(PSTR("\r\nread root directory:"));
   offset = 0;
   while (1) {
      SD_read(root_sector,offset,buffer,32);
      memcpy(&attribute,buffer+11,1);
      put_flash_string(PSTR("\r\n   attribute: 0x"));
      put_hex_char(attribute);
      if (attribute == 0xF) {
         //
         // long file name
         //
         put_flash_string(PSTR("\r\n   long file name: 0x"));
         put_hex_string(buffer+1,10);
         put_hex_string(buffer+14,12);
         put_hex_string(buffer+28,4);
         put_flash_string(PSTR("\r\n                   "));
         put_char_string(buffer+1,10);
         put_char_string(buffer+14,12);
         put_char_string(buffer+28,4);
         }
      else {
         //
         // short file name
         //
         put_flash_string(PSTR("\r\n   short file name: 0x"));
         put_hex_string(buffer,11);
         put_flash_string(PSTR("\r\n                    "));
         put_char_string(buffer,11);
         //
         memcpy(&file_cluster_hi,buffer+20,2);
         memcpy(&file_cluster_low,buffer+26,2);
         file_cluster = (((uint32_t) file_cluster_hi) << 16)+file_cluster_low;
         put_flash_string(PSTR("\r\n   file cluster: 0x"));
         put_hex_char((file_cluster >> 24) & 0xFF);
         put_hex_char((file_cluster >> 16) & 0xFF);
         put_hex_char((file_cluster >> 8) & 0xFF);
         put_hex_char(file_cluster & 0xFF);
         //
         memcpy(&file_length,buffer+28,4);
         put_flash_string(PSTR("\r\n   file length: 0x"));
         put_hex_char((file_length >> 24) & 0xFF);
         put_hex_char((file_length >> 16) & 0xFF);
         put_hex_char((file_length >> 8) & 0xFF);
         put_hex_char(file_length & 0xFF);
         //
         // break if not unused
         //
         if (buffer[0] != 0xE5)
            break;
         }
      offset += 32;
      }
   //
   // read the file
   //
   put_flash_string(PSTR("\r\nread first file:\r\n\r\n"));
   file_sector = root_sector+(file_cluster-2)*sectors_per_cluster;
   buffer_length = 32;
   offset = 0;
   sector_count = 0;
   chars_read = 0;
   while (1) {
      //
      // check file length
      //
      if ((chars_read+buffer_length) > file_length) {
         SD_read(file_sector,offset,buffer,file_length-chars_read);
         put_char_string(buffer,file_length-chars_read);
         break;
         }
      SD_read(file_sector,offset,buffer,buffer_length);
      put_char_string(buffer,buffer_length);
      chars_read += buffer_length;
      offset += buffer_length;
      //
      // check sector length
      //
      if (offset == bytes_per_sector) {
         //
         // sector length reached, check cluster length
         //
         sector_count += 1;
         if (sector_count == sectors_per_cluster) {
            //
            // cluster length reached, get new cluster from FAT
            //
            SD_read(fat_sector,4*file_cluster,buffer,4);
            memcpy(&file_cluster,buffer,4);
            //
            // continue
            //
            file_sector = root_sector+(file_cluster-2)*sectors_per_cluster;
            offset = 0;
            sector_count = 0;
            }
         else {
            //
            // cluster length not reached, continue
            //
            file_sector += 1;
            offset = 0;
            }
         }
      //
      // sector length not reached, continue
      //
      }
   put_flash_string(PSTR("\r\n\r\nend of file\r\n"));
   }
