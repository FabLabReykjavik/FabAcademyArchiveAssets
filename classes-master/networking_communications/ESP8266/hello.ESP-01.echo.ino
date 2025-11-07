//
// hello.ESP-01.echo.ino
//
// ESP-01 echo hello-world
//
// Neil Gershenfeld 1/6/20
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//

void setup() {
   Serial.begin(115200);
   }

#define max_buffer 25

void loop() {
   static char chr;
   static char buffer[max_buffer] = {0};
   static int index;
   if (Serial.available()) {
      chr = Serial.read();
      Serial.print("hello.ESP-01.echo: you typed \"");
      buffer[index++] = chr;
      if (index == (max_buffer-1))
         index = 0;
      Serial.print(buffer);
      Serial.println("\"");
      }
   }
