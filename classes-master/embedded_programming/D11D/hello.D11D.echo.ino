//
// hello.D11D.echo.ino
//
// ATSAMD11D USB echo hello-world
//
// Neil Gershenfeld 11/29/19
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

void setup() {
   SerialUSB.begin(0);
   }

#define max_buffer 25

void loop() {
   static char chr;
   static char buffer[max_buffer] = {0};
   static int index;
   if (SerialUSB.available()) {
      chr = SerialUSB.read();
      SerialUSB.print("hello.D11D.echo: you typed \"");
      buffer[index++] = chr;
      if (index == (max_buffer-1))
         index = 0;
      SerialUSB.print(buffer);
      SerialUSB.println("\"");
      }
   }
