//
// hello.uSD.D11C.ino
//
// D11C microSD hello-world
//
// Neil Gershenfeld 12/30/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

#include <SPI.h>
#include <SD.h>

File file,dir;
char chr;

void setup() {
   SerialUSB.begin(0); // start USB
   while (!SerialUSB) {;}; // wait for USB to start
   SD.begin(15); // start SD with SS on A15
   file = SD.open("hello.txt",FILE_WRITE); // open file for writing
   file.println("hello world"); // write to file
   file.close(); // close file
   dir = SD.open("/"); // open root directory
   file = dir.openNextFile(); // open first file
   SerialUSB.print("reading ");
   SerialUSB.print(file.name()); // get file name
   SerialUSB.println(":");
   while (file.available()) // loop over file
      SerialUSB.write(file.read()); // print contents
   SD.remove("hello.txt"); // remove file
   }

void loop() {
   ; // nothing to do here
   }
