//
// hello.button-blink.C3.ino
//
// Seeed XIAO ESP32C3 button, blink,  and echo hello-world
//
// Neil Gershenfeld 10/8/23
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge this project. Copyright is retained and
// must be preserved. The work is provided as is; no
// warranty is provided, and users accept all liability.
//
// add XIAO_ESP32C3 board
//    https://espressif.github.io/arduino-esp32/package_esp32_index.json
//

#define led_pin 20
#define button_pin 21

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
