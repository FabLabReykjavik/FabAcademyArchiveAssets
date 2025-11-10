//
// hello.GPS.t1614.ino
//
// GPS ATtiny1614 hello-world
//
// Neil Gershenfeld 1/3/21
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <SoftwareSerial.h>
#include <LibPrintf.h>
#include <string.h>

#define GPS_Rx 7
#define GPS_Tx 6
#define GPS_baud 9600

char c,buf[100];
char *ptr,*ID,*UTC,*latitude,*NS,*longitude,*EW,*fix,*satellites,*dilation,*altitude,*units;
uint8_t len = 0;
SoftwareSerial GPS(GPS_Rx,GPS_Tx);

void setup() {
   Serial.begin(115200);
   GPS.begin(GPS_baud);
   }

void loop() {
   while (1) {
      if (GPS.available() != 0) { // check for data
         c = GPS.read();
         buf[len++] = c;
         }
      if (c == '\n') { // check for end of line
         len = 0;
         if (strncmp(buf,"$GPGGA",6) == 0) { // check for fix data
            ID = strtok_r(buf,",",&ptr);
            UTC = strtok_r(NULL,",",&ptr);
            if (buf[10+UTC-buf] == ',') { // check for empty latitude
               printf("%s: no fix\n",UTC);
               break;
               }
            latitude = strtok_r(NULL,",",&ptr);
            NS = strtok_r(NULL,",",&ptr);
            longitude = strtok_r(NULL,",",&ptr);
            EW = strtok_r(NULL,",",&ptr);
            fix = strtok_r(NULL,",",&ptr);
            satellites = strtok_r(NULL,",",&ptr);
            dilation = strtok_r(NULL,",",&ptr);
            altitude = strtok_r(NULL,",",&ptr);
            units = strtok_r(NULL,",",&ptr);
            printf("UTC time: %s, ",UTC);
            printf("Latitude: %s ",latitude);
            printf("%s, ",NS);
            printf("Longitude: %s ",longitude);
            printf("%s, ",EW);
            printf("Satellites: %s, ",satellites);
            printf("Altitude: %s ",altitude);
            printf("%s\n",units);
            break;
            }
         }
      }
   }
