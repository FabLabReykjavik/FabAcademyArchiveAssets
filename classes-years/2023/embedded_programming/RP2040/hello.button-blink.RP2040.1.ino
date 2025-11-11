//
// hello.button-blink.RP2040.1.ino
//
// Seeed XIAO RP2040 button, blink, echo hello-world, single core
//
// Neil Gershenfeld 12/28/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//
// add RP2040 core
//    https://github.com/earlephilhower/arduino-pico
//

#define led_pin 1
#define button_pin 0

void setup() {
   pinMode(led_pin,OUTPUT);
   pinMode(button_pin,INPUT_PULLUP);
   Serial.begin();
   Serial.setTimeout(10);
   }

bool button_up = true;

void loop() {
   if (Serial.available()) {
      digitalWrite(led_pin,HIGH);
      String s = Serial.readString();
      Serial.print("you typed: ");
      Serial.println(s);
      delay(100);
      digitalWrite(led_pin,LOW);
      }
   if ((digitalRead(button_pin) == LOW) && button_up) {
      digitalWrite(led_pin,HIGH);
      Serial.println("button down");
      button_up = false;
      }
   else if ((digitalRead(button_pin) == HIGH) && !button_up) {
      digitalWrite(led_pin,LOW);
      Serial.println("button up");
      button_up = true;
      }
   }
