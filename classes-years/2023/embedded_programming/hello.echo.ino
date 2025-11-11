//
// hello.echo.ino
//
// serial echo hello-world
//
// Neil Gershenfeld 2/20/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//
// globals
//
#define bufsize 25
char buf[bufsize];
int count=0;
//
// setup
//
void setup() {
   Serial.begin(115200);
   }
//
// main loop
//
void loop() {
   char chr;
   //
   // check for a char
   //
   if (Serial.available()) {
      //
      // read, save, and send char
      //
      chr = Serial.read();
      buf[count] = chr;
      count += 1;
      buf[count] = 0;
      if (count == (bufsize-1))
         count = 0;
      Serial.print("hello.echo.ino: you typed ");
      Serial.println(buf);
      }
   }
